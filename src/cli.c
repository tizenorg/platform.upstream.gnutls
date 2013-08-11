/*
 * Copyright (C) 2000-2012 Free Software Foundation, Inc.
 *
 * This file is part of GnuTLS.
 *
 * GnuTLS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GnuTLS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#if HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#elif HAVE_WS2TCPIP_H
# include <ws2tcpip.h>
#endif
#include <sys/select.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <netdb.h>
#include <ctype.h>

#include <gnutls/gnutls.h>
#include <gnutls/abstract.h>
#include <gnutls/dtls.h>
#include <gnutls/x509.h>
#include <gnutls/openpgp.h>
#include <gnutls/pkcs11.h>

/* Gnulib portability files. */
#include <progname.h>
#include <version-etc.h>
#include <read-file.h>
#include <getpass.h>
#include <minmax.h>

#include "sockets.h"
#include "benchmark.h"

#include <common.h>
#include <socket.h>

#include <cli-args.h>
#include <ocsptool-common.h>

#define MAX_BUF 4096

/* global stuff here */
int resume, starttls, insecure, rehandshake, udp, mtu;
const char *hostname = NULL;
const char *service = NULL;
int record_max_size;
int fingerprint;
int crlf;
unsigned int verbose = 0;
int print_cert;

const char *srp_passwd = NULL;
const char *srp_username = NULL;
const char *pgp_keyfile = NULL;
const char *pgp_certfile = NULL;
const char *pgp_keyring = NULL;
const char *x509_keyfile = NULL;
const char *x509_certfile = NULL;
const char *x509_cafile = NULL;
const char *x509_crlfile = NULL;
static int x509ctype;
static int disable_extensions;
static const char * priorities = NULL;

const char *psk_username = NULL;
gnutls_datum_t psk_key = { NULL, 0 };

static gnutls_srp_client_credentials_t srp_cred;
static gnutls_psk_client_credentials_t psk_cred;
static gnutls_anon_client_credentials_t anon_cred;
static gnutls_certificate_credentials_t xcred;

/* end of global stuff */

/* prototypes */

static void check_rehandshake (socket_st * socket, int ret);
static int do_handshake (socket_st * socket);
static void init_global_tls_stuff (void);
static int cert_verify_ocsp (gnutls_session_t session);

/* Helper functions to load a certificate and key
 * files into memory.
 */
static gnutls_datum_t
load_file (const char *file)
{
  gnutls_datum_t loaded_file = { NULL, 0 };
  size_t length;

  loaded_file.data = (void*)read_binary_file (file, &length);
  if (loaded_file.data)
    loaded_file.size = (unsigned int) length;

  return loaded_file;
}

static void
unload_file (gnutls_datum_t* data)
{
  free (data->data);
}

#define MAX_CRT 6
static unsigned int x509_crt_size;
static gnutls_pcert_st x509_crt[MAX_CRT];
static gnutls_privkey_t x509_key = NULL;

static gnutls_pcert_st pgp_crt;
static gnutls_privkey_t pgp_key = NULL;

static void
get_keyid (gnutls_openpgp_keyid_t keyid, const char *str)
{
  size_t keyid_size = sizeof (keyid);

  if (strlen (str) != 16)
    {
      fprintf (stderr,
               "The OpenPGP subkey ID has to be 16 hexadecimal characters.\n");
      exit (1);
    }

  if (gnutls_hex2bin (str, strlen (str), keyid, &keyid_size) < 0)
    {
      fprintf (stderr, "Error converting hex string: %s.\n", str);
      exit (1);
    }

  return;
}

/* Load the certificate and the private key.
 */
static void
load_keys (void)
{
  unsigned int crt_num;
  int ret;
  unsigned int i;
  gnutls_datum_t data = { NULL, 0 };
  gnutls_x509_crt_t crt_list[MAX_CRT];
#ifdef ENABLE_PKCS11
  gnutls_pkcs11_privkey_t pkcs11_key;
#endif
  gnutls_x509_privkey_t tmp_key;
  unsigned char keyid[GNUTLS_OPENPGP_KEYID_SIZE];

  if (x509_certfile != NULL && x509_keyfile != NULL)
    {
#ifdef ENABLE_PKCS11
      if (strncmp (x509_certfile, "pkcs11:", 7) == 0)
        {
          crt_num = 1;
          gnutls_x509_crt_init (&crt_list[0]);

          ret =
            gnutls_x509_crt_import_pkcs11_url (crt_list[0], x509_certfile, 0);

          if (ret == GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE)
            ret =
              gnutls_x509_crt_import_pkcs11_url (crt_list[0], x509_certfile,
                                                 GNUTLS_PKCS11_OBJ_FLAG_LOGIN);

          if (ret < 0)
            {
              fprintf (stderr, "*** Error loading cert file.\n");
              exit (1);
            }
          x509_crt_size = 1;
        }
      else
#endif /* ENABLE_PKCS11 */
        {

          data = load_file (x509_certfile);
          if (data.data == NULL)
            {
              fprintf (stderr, "*** Error loading cert file.\n");
              exit (1);
            }

          crt_num = MAX_CRT;
          ret =
            gnutls_x509_crt_list_import (crt_list, &crt_num, &data,
                                         x509ctype,
                                         GNUTLS_X509_CRT_LIST_IMPORT_FAIL_IF_EXCEED);
          if (ret < 0)
            {
              if (ret == GNUTLS_E_SHORT_MEMORY_BUFFER)
                {
                  fprintf (stderr,
                           "*** Error loading cert file: Too many certs %d\n",
                           crt_num);

                }
              else
                {
                  fprintf (stderr,
                           "*** Error loading cert file: %s\n",
                           gnutls_strerror (ret));
                }
              exit (1);
            }
          x509_crt_size = ret;
        }
      
      for (i=0;i<x509_crt_size;i++)
        {
          ret = gnutls_pcert_import_x509(&x509_crt[i], crt_list[i], 0);
          if (ret < 0)
            {
              fprintf(stderr, "*** Error importing crt to pcert: %s\n",
                gnutls_strerror(ret));
              exit(1);
            }
          gnutls_x509_crt_deinit(crt_list[i]);
        }

      unload_file (&data);

      ret = gnutls_privkey_init(&x509_key);
      if (ret < 0)
         {
           fprintf (stderr, "*** Error initializing key: %s\n",
                    gnutls_strerror (ret));
           exit (1);
         }

#ifdef ENABLE_PKCS11
      if (strncmp (x509_keyfile, "pkcs11:", 7) == 0)
        {
          gnutls_pkcs11_privkey_init (&pkcs11_key);

          ret =
            gnutls_pkcs11_privkey_import_url (pkcs11_key, x509_keyfile, 0);
          if (ret < 0)
            {
              fprintf (stderr, "*** Error loading url: %s\n",
                       gnutls_strerror (ret));
              exit (1);
            }

          ret = gnutls_privkey_import_pkcs11( x509_key, pkcs11_key, GNUTLS_PRIVKEY_IMPORT_AUTO_RELEASE);
          if (ret < 0)
            {
              fprintf (stderr, "*** Error loading url: %s\n",
                       gnutls_strerror (ret));
              exit (1);
            }
        }
      else
#endif /* ENABLE_PKCS11 */
        {
          data = load_file (x509_keyfile);
          if (data.data == NULL)
            {
              fprintf (stderr, "*** Error loading key file.\n");
              exit (1);
            }

          gnutls_x509_privkey_init (&tmp_key);

          ret =
            gnutls_x509_privkey_import (tmp_key, &data, x509ctype);
          if (ret < 0)
            {
              fprintf (stderr, "*** Error loading key file: %s\n",
                       gnutls_strerror (ret));
              exit (1);
            }

          ret = gnutls_privkey_import_x509( x509_key, tmp_key, GNUTLS_PRIVKEY_IMPORT_AUTO_RELEASE);
          if (ret < 0)
            {
              fprintf (stderr, "*** Error loading url: %s\n",
                       gnutls_strerror (ret));
              exit (1);
            }

          unload_file (&data);
        }

      fprintf (stdout, "Processed %d client X.509 certificates...\n",
               x509_crt_size);
    }


#ifdef ENABLE_OPENPGP
  if (HAVE_OPT(PGPSUBKEY))
    {
      get_keyid (keyid, OPT_ARG(PGPSUBKEY));
    }

  if (pgp_certfile != NULL && pgp_keyfile != NULL)
    {
      gnutls_openpgp_crt_t tmp_pgp_crt;

      data = load_file (pgp_certfile);
      if (data.data == NULL)
        {
          fprintf (stderr, "*** Error loading PGP cert file.\n");
          exit (1);
        }

      gnutls_openpgp_crt_init (&tmp_pgp_crt);

      ret =
        gnutls_pcert_import_openpgp_raw (&pgp_crt, &data, GNUTLS_OPENPGP_FMT_BASE64, HAVE_OPT(PGPSUBKEY)?keyid:NULL, 0);
      if (ret < 0)
        {
          fprintf (stderr,
                   "*** Error loading PGP cert file: %s\n",
                   gnutls_strerror (ret));
          exit (1);
        }
 
      unload_file (&data);

      ret = gnutls_privkey_init(&pgp_key);
      if (ret < 0)
         {
           fprintf (stderr, "*** Error initializing key: %s\n",
                    gnutls_strerror (ret));
           exit (1);
         }

#ifdef ENABLE_PKCS11
      if (strncmp (pgp_keyfile, "pkcs11:", 7) == 0)
        {
          gnutls_pkcs11_privkey_init (&pkcs11_key);

          ret = gnutls_pkcs11_privkey_import_url (pkcs11_key, pgp_keyfile, 0);
          if (ret < 0)
            {
              fprintf (stderr, "*** Error loading url: %s\n",
                       gnutls_strerror (ret));
              exit (1);
            }

          ret = gnutls_privkey_import_pkcs11( pgp_key, pkcs11_key, GNUTLS_PRIVKEY_IMPORT_AUTO_RELEASE);
          if (ret < 0)
            {
              fprintf (stderr, "*** Error loading url: %s\n",
                       gnutls_strerror (ret));
              exit (1);
            }
        }
      else
#endif /* ENABLE_PKCS11 */
        {
          gnutls_openpgp_privkey_t tmp_pgp_key;

          data = load_file (pgp_keyfile);
          if (data.data == NULL)
            {
              fprintf (stderr, "*** Error loading PGP key file.\n");
              exit (1);
            }

          gnutls_openpgp_privkey_init (&tmp_pgp_key);

          ret =
            gnutls_openpgp_privkey_import (tmp_pgp_key, &data,
                                           GNUTLS_OPENPGP_FMT_BASE64, NULL,
                                           0);
          if (ret < 0)
            {
              fprintf (stderr,
                       "*** Error loading PGP key file: %s\n",
                       gnutls_strerror (ret));
              exit (1);
            }

          if (HAVE_OPT(PGPSUBKEY))
            {
              ret =
                gnutls_openpgp_privkey_set_preferred_key_id (tmp_pgp_key, keyid);
              if (ret < 0)
                {
                  fprintf (stderr,
                      "*** Error setting preferred sub key id (%s): %s\n",
                      OPT_ARG(PGPSUBKEY), gnutls_strerror (ret));
                  exit (1);
                }
            }

          ret = gnutls_privkey_import_openpgp( pgp_key, tmp_pgp_key, GNUTLS_PRIVKEY_IMPORT_AUTO_RELEASE);
          if (ret < 0)
            {
              fprintf (stderr, "*** Error loading url: %s\n",
                       gnutls_strerror (ret));
              exit (1);
            }

          unload_file (&data);
        }


      fprintf (stdout, "Processed 1 client PGP certificate...\n");
    }
#endif

}

#define IS_NEWLINE(x) ((x[0] == '\n') || (x[0] == '\r'))
static int
read_yesno (const char *input_str)
{
  char input[128];

  fputs (input_str, stderr);
  if (fgets (input, sizeof (input), stdin) == NULL)
    return 0;

  if (IS_NEWLINE(input))
    return 0;

  if (input[0] == 'y' || input[0] == 'Y')
    return 1;

  return 0;
}

/* converts a textual service or port to
 * a service.
 */
static const char* port_to_service(const char* sport)
{
unsigned int port;
struct servent * sr;
  
  port = atoi(sport);
  if (port == 0) return sport;

  port = htons(port);

  sr = getservbyport(port, udp?"udp":"tcp");
  if (sr == NULL)
    {
      fprintf(stderr, "Warning: getservbyport() failed. Using port number as service.\n");
      return sport;
    }

  return sr->s_name;
}

static int
cert_verify_callback (gnutls_session_t session)
{
  int rc;
  unsigned int status = 0;
  int ssh = ENABLED_OPT(TOFU);
  const char* txt_service;

  rc = cert_verify(session, hostname);
  if (rc == 0)
    {
      printf ("*** Verifying server certificate failed...\n");
      if (!insecure && !ssh)
        return -1;
    }
  else if (ENABLED_OPT(OCSP))
    { /* off-line verification succeeded. Try OCSP */
      rc = cert_verify_ocsp(session);
      if (rc == 0)
        {
          printf ("*** Verifying (with OCSP) server certificate failed...\n");
          if (!insecure && !ssh)
            return -1;
        }
      else if (rc == -1)
        printf("*** OCSP response ignored\n");
    }

  if (ssh) /* try ssh auth */
    {
      unsigned int list_size;
      const gnutls_datum_t * cert;
      
      cert = gnutls_certificate_get_peers(session, &list_size);
      if (cert == NULL)
        {
          fprintf(stderr, "Cannot obtain peer's certificate!\n");
          return -1;
        }

      txt_service = port_to_service(service);
      
      rc = gnutls_verify_stored_pubkey(NULL, NULL, hostname, txt_service, 
                                       GNUTLS_CRT_X509, cert, 0);
      if (rc == GNUTLS_E_NO_CERTIFICATE_FOUND)
        {
          print_cert_info_compact(session);
          fprintf(stderr, "Host %s (%s) has never been contacted before.\n", hostname, txt_service);
          if (status == 0)
            fprintf(stderr, "Its certificate is valid for %s.\n", hostname);

          rc = read_yesno("Are you sure you want to trust it? (y/N): ");
          if (rc == 0)
            return -1;
        }
      else if (rc == GNUTLS_E_CERTIFICATE_KEY_MISMATCH)
        {
          print_cert_info_compact(session);
          fprintf(stderr, "Warning: host %s is known and it is associated with a different key.\n", hostname);
          fprintf(stderr, "It might be that the server has multiple keys, or an attacker replaced the key to eavesdrop this connection .\n");
          if (status == 0)
            fprintf(stderr, "Its certificate is valid for %s.\n", hostname);

          rc = read_yesno("Do you trust the received key? (y/N): ");
          if (rc == 0)
            return -1;
        }
      else if (rc < 0)
        {
          fprintf(stderr, "gnutls_verify_stored_pubkey: %s\n", gnutls_strerror(rc));
          return -1;
        }
      
      if (rc != 0)
        {
          rc = gnutls_store_pubkey(NULL, NULL, hostname, txt_service, 
                                   GNUTLS_CRT_X509, cert, 0, 0);
          if (rc < 0)
            fprintf(stderr, "Could not store key: %s\n", gnutls_strerror(rc));
        }
    }

  return 0;
}

/* This callback should be associated with a session by calling
 * gnutls_certificate_client_set_retrieve_function( session, cert_callback),
 * before a handshake.
 */

static int
cert_callback (gnutls_session_t session,
               const gnutls_datum_t * req_ca_rdn, int nreqs,
               const gnutls_pk_algorithm_t * sign_algos,
               int sign_algos_length, gnutls_pcert_st **pcert,
               unsigned int *pcert_length, gnutls_privkey_t * pkey)
{
  char issuer_dn[256];
  int i, ret, cert_type;
  size_t len;

  if (verbose)
    {
      /* Print the server's trusted CAs
       */
      if (nreqs > 0)
        printf ("- Server's trusted authorities:\n");
      else
        printf ("- Server did not send us any trusted authorities names.\n");

      /* print the names (if any) */
      for (i = 0; i < nreqs; i++)
        {
          len = sizeof (issuer_dn);
          ret = gnutls_x509_rdn_get (&req_ca_rdn[i], issuer_dn, &len);
          if (ret >= 0)
            {
              printf ("   [%d]: ", i);
              printf ("%s\n", issuer_dn);
            }
        }
    }

  /* Select a certificate and return it.
   * The certificate must be of any of the "sign algorithms"
   * supported by the server.
   */

  cert_type = gnutls_certificate_type_get (session);

  *pcert_length = 0;

  if (cert_type == GNUTLS_CRT_X509)
    {
      if (x509_crt_size > 0)
        {
          if (x509_key != NULL)
            {
              *pkey = x509_key;
            }
          else
            {
              printf ("- Could not find a suitable key to send to server\n");
              return -1;
            }

          *pcert_length = x509_crt_size;
          *pcert = x509_crt;
        }

    }
  else if (cert_type == GNUTLS_CRT_OPENPGP)
    {
      if (pgp_key != NULL)
        {
          *pkey = pgp_key;

          *pcert_length = 1;
          *pcert = &pgp_crt;
        }
    }

  printf ("- Successfully sent %u certificate(s) to server.\n", *pcert_length);
  return 0;

}

/* initializes a gnutls_session_t with some defaults.
 */
static gnutls_session_t
init_tls_session (const char *hostname)
{
  const char *err;
  int ret;
  gnutls_session_t session;

  if (priorities == NULL)
    priorities = "NORMAL";
  
  if (udp)
    {
      gnutls_init (&session, GNUTLS_CLIENT|GNUTLS_DATAGRAM);
      if (mtu)
        gnutls_dtls_set_mtu(session, mtu);
    }
  else
    gnutls_init (&session, GNUTLS_CLIENT);

  if ((ret = gnutls_priority_set_direct (session, priorities, &err)) < 0)
    {
      if (ret == GNUTLS_E_INVALID_REQUEST) fprintf (stderr, "Syntax error at: %s\n", err);
      else 
        fprintf(stderr, "Error in priorities: %s\n", gnutls_strerror(ret));
      exit (1);
    }

  /* allow the use of private ciphersuites.
   */
  if (disable_extensions == 0)
    {
      if (!isdigit(hostname[0]) && strchr(hostname, ':') == 0)
        gnutls_server_name_set (session, GNUTLS_NAME_DNS, hostname,
                                strlen (hostname));
    }

  gnutls_dh_set_prime_bits (session, 512);

  gnutls_credentials_set (session, GNUTLS_CRD_ANON, anon_cred);
  if (srp_cred)
    gnutls_credentials_set (session, GNUTLS_CRD_SRP, srp_cred);
  if (psk_cred)
    gnutls_credentials_set (session, GNUTLS_CRD_PSK, psk_cred);
  gnutls_credentials_set (session, GNUTLS_CRD_CERTIFICATE, xcred);

  gnutls_certificate_set_retrieve_function2 (xcred, cert_callback);
  gnutls_certificate_set_verify_function (xcred, cert_verify_callback);

  /* send the fingerprint */
#ifdef ENABLE_OPENPGP
  if (fingerprint != 0)
    gnutls_openpgp_send_cert (session, GNUTLS_OPENPGP_CERT_FINGERPRINT);
#endif

  /* use the max record size extension */
  if (record_max_size > 0 && disable_extensions == 0)
    {
      if (gnutls_record_set_max_size (session, record_max_size) < 0)
        {
          fprintf (stderr,
                   "Cannot set the maximum record size to %d.\n",
                   record_max_size);
          fprintf (stderr, "Possible values: 512, 1024, 2048, 4096.\n");
          exit (1);
        }
    }

#ifdef ENABLE_SESSION_TICKET
  if (disable_extensions == 0 && !HAVE_OPT(NOTICKET)t)
    gnutls_session_ticket_enable_client (session);
#endif

  return session;
}

static void cmd_parser (int argc, char **argv);

/* Returns zero if the error code was successfully handled.
 */
static int
handle_error (socket_st * hd, int err)
{
  int alert, ret;
  const char *err_type, *str;

  if (err >= 0 || err == GNUTLS_E_AGAIN || err == GNUTLS_E_INTERRUPTED)
    return 0;

  if (gnutls_error_is_fatal (err) == 0)
    {
      ret = 0;
      err_type = "Non fatal";
    }
  else
    {
      ret = err;
      err_type = "Fatal";
    }

  str = gnutls_strerror (err);
  if (str == NULL)
    str = str_unknown;
  fprintf (stderr, "*** %s error: %s\n", err_type, str);

  if (err == GNUTLS_E_WARNING_ALERT_RECEIVED
      || err == GNUTLS_E_FATAL_ALERT_RECEIVED)
    {
      alert = gnutls_alert_get (hd->session);
      str = gnutls_alert_get_name (alert);
      if (str == NULL)
        str = str_unknown;
      printf ("*** Received alert [%d]: %s\n", alert, str);
    }

  check_rehandshake (hd, err);

  return ret;
}

int starttls_alarmed = 0;

#ifndef _WIN32
static void
starttls_alarm (int signum)
{
  starttls_alarmed = 1;
}
#endif

static void
tls_log_func (int level, const char *str)
{
  fprintf (stderr, "|<%d>| %s", level, str);
}

#define IN_KEYBOARD 1
#define IN_NET 2
#define IN_NONE 0
/* returns IN_KEYBOARD for keyboard input and IN_NET for network input
 */
static int check_net_or_keyboard_input(socket_st* hd)
{
  int maxfd;
  fd_set rset;
  int err;
  struct timeval tv;

  do
    {
      FD_ZERO (&rset);
      FD_SET (hd->fd, &rset);

#ifndef _WIN32
      FD_SET (fileno (stdin), &rset);
      maxfd = MAX (fileno (stdin), hd->fd);
#else
      maxfd = hd->fd;
#endif

      tv.tv_sec = 0;
      tv.tv_usec = 50 * 1000;

      if (hd->secure == 1)
        if (gnutls_record_check_pending(hd->session))
          return IN_NET;

      err = select (maxfd + 1, &rset, NULL, NULL, &tv);
      if (err < 0)
        continue;

      if (FD_ISSET (hd->fd, &rset))
        return IN_NET;

#ifdef _WIN32
      {
        int state;
        state = WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 200); 
        
        if (state == WAIT_OBJECT_0)
          return IN_KEYBOARD;
      }
#else
      if (FD_ISSET (fileno (stdin), &rset))
        return IN_KEYBOARD;
#endif
    }
  while(err == 0);
  
  return IN_NONE;
}

int
main (int argc, char **argv)
{
  int ret;
  int ii, i, inp;
  char buffer[MAX_BUF + 1];
  char *session_data = NULL;
  char *session_id = NULL;
  size_t session_data_size;
  size_t session_id_size = 0;
  int user_term = 0, retval = 0;
  socket_st hd;
  ssize_t bytes;

  set_program_name (argv[0]);
  cmd_parser (argc, argv);

  gnutls_global_set_log_function (tls_log_func);
  gnutls_global_set_log_level (OPT_VALUE_DEBUG);

  if ((ret = gnutls_global_init ()) < 0)
    {
      fprintf (stderr, "global_init: %s\n", gnutls_strerror (ret));
      exit (1);
    }

#ifdef ENABLE_PKCS11
  pkcs11_common ();
#endif

  if (hostname == NULL)
    {
      fprintf (stderr, "No hostname given\n");
      exit (1);
    }

  sockets_init ();

  init_global_tls_stuff ();

  socket_open (&hd, hostname, service, udp);
  socket_connect (&hd);

  hd.session = init_tls_session (hostname);
  if (starttls)
    goto after_handshake;

  for (i = 0; i < 2; i++)
    {


      if (i == 1)
        {
          hd.session = init_tls_session (hostname);
          gnutls_session_set_data (hd.session, session_data,
                                   session_data_size);
          free (session_data);
        }

      ret = do_handshake (&hd);

      if (ret < 0)
        {
          fprintf (stderr, "*** Handshake has failed\n");
          gnutls_perror (ret);
          gnutls_deinit (hd.session);
          return 1;
        }
      else
        {
          printf ("- Handshake was completed\n");
          if (gnutls_session_is_resumed (hd.session) != 0)
            printf ("*** This is a resumed session\n");
        }

      if (resume != 0 && i == 0)
        {

          gnutls_session_get_data (hd.session, NULL, &session_data_size);
          session_data = malloc (session_data_size);

          gnutls_session_get_data (hd.session, session_data,
                                   &session_data_size);

          gnutls_session_get_id (hd.session, NULL, &session_id_size);

          session_id = malloc (session_id_size);
          gnutls_session_get_id (hd.session, session_id, &session_id_size);

          printf ("- Disconnecting\n");
          socket_bye (&hd);

          printf
            ("\n\n- Connecting again- trying to resume previous session\n");
          socket_open (&hd, hostname, service, udp);
          socket_connect (&hd);
        }
      else
        {
          break;
        }
    }

after_handshake:

  /* Warning!  Do not touch this text string, it is used by external
     programs to search for when gnutls-cli has reached this point. */
  printf ("\n- Simple Client Mode:\n\n");

  if (rehandshake)
    {
      ret = do_handshake (&hd);

      if (ret < 0)
        {
          fprintf (stderr, "*** ReHandshake has failed\n");
          gnutls_perror (ret);
          gnutls_deinit (hd.session);
          return 1;
        }
      else
        {
          printf ("- ReHandshake was completed\n");
        }
    }

#ifndef _WIN32
  signal (SIGALRM, starttls_alarm);
#endif

  fflush (stdout);
  fflush (stderr);

  /* do not buffer */
#if !(defined _WIN32 || defined __WIN32__)
  setbuf (stdin, NULL);
#endif
  setbuf (stdout, NULL);
  setbuf (stderr, NULL);

  for (;;)
    {
      if (starttls_alarmed && !hd.secure)
        {
          /* Warning!  Do not touch this text string, it is used by
             external programs to search for when gnutls-cli has
             reached this point. */
          fprintf (stderr, "*** Starting TLS handshake\n");
          ret = do_handshake (&hd);
          if (ret < 0)
            {
              fprintf (stderr, "*** Handshake has failed\n");
              user_term = 1;
              retval = 1;
              break;
            }
        }

      inp = check_net_or_keyboard_input(&hd);

      if (inp == IN_NET)
        {
          memset (buffer, 0, MAX_BUF + 1);
          ret = socket_recv (&hd, buffer, MAX_BUF);

          if (ret == 0)
            {
              printf ("- Peer has closed the GnuTLS connection\n");
              break;
            }
          else if (handle_error (&hd, ret) < 0 && user_term == 0)
            {
              fprintf (stderr,
                       "*** Server has terminated the connection abnormally.\n");
              retval = 1;
              break;
            }
          else if (ret > 0)
            {
              if (verbose != 0)
                printf ("- Received[%d]: ", ret);
              for (ii = 0; ii < ret; ii++)
                {
                  fputc (buffer[ii], stdout);
                }
              fflush (stdout);
            }

          if (user_term != 0)
            break;
        }

      if (inp == IN_KEYBOARD)
        {
          if ((bytes = read (fileno (stdin), buffer, MAX_BUF - 1)) <= 0)
            {
              if (hd.secure == 0)
                {
                  /* Warning!  Do not touch this text string, it is
                     used by external programs to search for when
                     gnutls-cli has reached this point. */
                  fprintf (stderr, "*** Starting TLS handshake\n");
                  ret = do_handshake (&hd);
                  clearerr (stdin);
                  if (ret < 0)
                    {
                      fprintf (stderr, "*** Handshake has failed\n");
                      user_term = 1;
                      retval = 1;
                      break;
                    }
                }
              else
                {
                  user_term = 1;
                  break;
                }
              continue;
            }

          buffer[bytes] = 0;
          if (crlf != 0)
            {
              char *b = strchr (buffer, '\n');
              if (b != NULL)
                {
                  strcpy (b, "\r\n");
                  bytes++;
                }
            }

          ret = socket_send (&hd, buffer, bytes);

          if (ret > 0)
            {
              if (verbose != 0)
                printf ("- Sent: %d bytes\n", ret);
            }
          else
            handle_error (&hd, ret);

        }
    }

  if (user_term != 0)
    socket_bye (&hd);
  else
    gnutls_deinit (hd.session);

#ifdef ENABLE_SRP
  if (srp_cred)
    gnutls_srp_free_client_credentials (srp_cred);
#endif
#ifdef ENABLE_PSK
  if (psk_cred)
    gnutls_psk_free_client_credentials (psk_cred);
#endif

  gnutls_certificate_free_credentials (xcred);

#ifdef ENABLE_ANON
  gnutls_anon_free_client_credentials (anon_cred);
#endif

  gnutls_global_deinit ();

  return retval;
}

static void
cmd_parser (int argc, char **argv)
{
const char* rest = NULL;

  int optct = optionProcess( &gnutls_cliOptions, argc, argv);
  argc -= optct;
  argv += optct;
  
  if (rest == NULL && argc > 0)
    rest = argv[0];
    
  if (HAVE_OPT(BENCHMARK_CIPHERS))
    {
      benchmark_cipher(1, OPT_VALUE_DEBUG); 
      exit(0);
    }

  if (HAVE_OPT(BENCHMARK_SOFT_CIPHERS))
    {
      benchmark_cipher(0, OPT_VALUE_DEBUG); 
      exit(0);
    }

  if (HAVE_OPT(BENCHMARK_TLS))
    {
      benchmark_tls(OPT_VALUE_DEBUG); 
      exit(0);
    }

  if (HAVE_OPT(PRIORITY)) 
    {
      priorities = OPT_ARG(PRIORITY);
    } 
  verbose = HAVE_OPT( VERBOSE);
  if (verbose)
    print_cert = 1;
  else
    print_cert = HAVE_OPT( PRINT_CERT);
  
  if (HAVE_OPT(LIST))
    {
      print_list(priorities, verbose);
      exit(0);
    }

  disable_extensions = HAVE_OPT( DISABLE_EXTENSIONS);
  starttls = HAVE_OPT(STARTTLS);
  resume = HAVE_OPT(RESUME);
  rehandshake = HAVE_OPT(REHANDSHAKE);
  insecure = HAVE_OPT(INSECURE);

  udp = HAVE_OPT(UDP);
  mtu = OPT_VALUE_MTU;
  
  if (HAVE_OPT(PORT)) 
    {
      service = OPT_ARG(PORT);
    }
  else 
    {
      service = "443";
    }

  record_max_size = OPT_VALUE_RECORDSIZE;
  fingerprint = HAVE_OPT(FINGERPRINT);

  if (HAVE_OPT(X509FMTDER))
    x509ctype = GNUTLS_X509_FMT_DER;
  else
    x509ctype = GNUTLS_X509_FMT_PEM;

  if (HAVE_OPT(SRPUSERNAME))
    srp_username = OPT_ARG(SRPUSERNAME);
    
  if (HAVE_OPT(SRPPASSWD))
    srp_passwd = OPT_ARG(SRPPASSWD);
  
  if (HAVE_OPT(X509CAFILE))
    x509_cafile = OPT_ARG(X509CAFILE);
  
  if (HAVE_OPT(X509CRLFILE))
    x509_crlfile = OPT_ARG(X509CRLFILE);
    
  if (HAVE_OPT(X509KEYFILE))
    x509_keyfile = OPT_ARG(X509KEYFILE);
  
  if (HAVE_OPT(X509CERTFILE))
    x509_certfile = OPT_ARG(X509CERTFILE);
  
  if (HAVE_OPT(PGPKEYFILE))
    pgp_keyfile = OPT_ARG(PGPKEYFILE);
  
  if (HAVE_OPT(PGPCERTFILE))
    pgp_certfile = OPT_ARG(PGPCERTFILE);

  if (HAVE_OPT(PSKUSERNAME))
    psk_username = OPT_ARG(PSKUSERNAME);

  if (HAVE_OPT(PSKKEY))
    {
      psk_key.data = (unsigned char *) OPT_ARG(PSKKEY);
      psk_key.size = strlen (OPT_ARG(PSKKEY));
    }
  else
    psk_key.size = 0;

  if (HAVE_OPT(PGPKEYRING))
    pgp_keyring = OPT_ARG(PGPKEYRING);

  crlf = HAVE_OPT(CRLF);

  if (rest != NULL)
    hostname = rest;
    
  if (hostname == NULL)
    {
      fprintf(stderr, "No hostname specified\n");
      exit(1);
    }
}

void cli_version (void);

void
cli_version (void)
{
  const char *p = PACKAGE_NAME;
  if (strcmp (gnutls_check_version (NULL), PACKAGE_VERSION) != 0)
    p = PACKAGE_STRING;
  version_etc (stdout, program_name, p, gnutls_check_version (NULL),
               "Nikos Mavrogiannopoulos", (char *) NULL);
}


static void
check_rehandshake (socket_st * socket, int ret)
{
  if (socket->secure && ret == GNUTLS_E_REHANDSHAKE)
    {
      /* There is a race condition here. If application
       * data is sent after the rehandshake request,
       * the server thinks we ignored his request.
       * This is a bad design of this client.
       */
      printf ("*** Received rehandshake request\n");
      /* gnutls_alert_send( session, GNUTLS_AL_WARNING, GNUTLS_A_NO_RENEGOTIATION); */

      ret = do_handshake (socket);

      if (ret == 0)
        {
          printf ("*** Rehandshake was performed.\n");
        }
      else
        {
          printf ("*** Rehandshake Failed.\n");
        }
    }
}


static int
do_handshake (socket_st * socket)
{
  int ret;

  gnutls_transport_set_ptr (socket->session,
                            (gnutls_transport_ptr_t)
                            gl_fd_to_handle (socket->fd));
  do
    {
      ret = gnutls_handshake (socket->session);

      if (ret < 0)
        {
          handle_error (socket, ret);
        }
    }
  while (ret < 0 && gnutls_error_is_fatal (ret) == 0);

  if (ret == 0)
    {
      /* print some information */
      print_info (socket->session, print_cert);
      socket->secure = 1;
    }
  else
    {
      print_cert_info (socket->session, verbose, print_cert);
      gnutls_alert_send_appropriate (socket->session, ret);
      shutdown (socket->fd, SHUT_RDWR);
    }
  return ret;
}

static int
srp_username_callback (gnutls_session_t session,
                       char **username, char **password)
{
  if (srp_username == NULL || srp_passwd == NULL)
    {
      return -1;
    }

  *username = gnutls_strdup (srp_username);
  *password = gnutls_strdup (srp_passwd);

  return 0;
}

static int
psk_callback (gnutls_session_t session, char **username, gnutls_datum_t * key)
{
  const char *hint = gnutls_psk_client_get_hint (session);
  char *rawkey;
  char *passwd;
  int ret;
  size_t res_size;
  gnutls_datum_t tmp;

  printf ("- PSK client callback. ");
  if (hint)
    printf ("PSK hint '%s'\n", hint);
  else
    printf ("No PSK hint\n");

  if (HAVE_OPT(PSKUSERNAME))
    *username = gnutls_strdup (OPT_ARG(PSKUSERNAME));
  else
    {
      char *tmp = NULL;
      size_t n;

      printf ("Enter PSK identity: ");
      fflush (stdout);
      getline (&tmp, &n, stdin);

      if (tmp == NULL)
        {
          fprintf (stderr, "No username given, aborting...\n");
          return GNUTLS_E_INSUFFICIENT_CREDENTIALS;
        }

      if (tmp[strlen (tmp) - 1] == '\n')
        tmp[strlen (tmp) - 1] = '\0';
      if (tmp[strlen (tmp) - 1] == '\r')
        tmp[strlen (tmp) - 1] = '\0';

      *username = gnutls_strdup (tmp);
      free (tmp);
    }
  if (!*username)
    return GNUTLS_E_MEMORY_ERROR;

  passwd = getpass ("Enter key: ");
  if (passwd == NULL)
    {
      fprintf (stderr, "No key given, aborting...\n");
      return GNUTLS_E_INSUFFICIENT_CREDENTIALS;
    }

  tmp.data = (void*)passwd;
  tmp.size = strlen (passwd);

  res_size = tmp.size / 2 + 1;
  rawkey = gnutls_malloc (res_size);
  if (rawkey == NULL)
    return GNUTLS_E_MEMORY_ERROR;

  ret = gnutls_hex_decode (&tmp, rawkey, &res_size);
  if (ret < 0)
    {
      fprintf (stderr, "Error deriving password: %s\n",
               gnutls_strerror (ret));
      gnutls_free (*username);
      return ret;
    }

  key->data = (void*)rawkey;
  key->size = res_size;

  if (HAVE_OPT(DEBUG))
    {
      char hexkey[41];
      res_size = sizeof (hexkey);
      gnutls_hex_encode (key, hexkey, &res_size);
      fprintf (stderr, "PSK username: %s\n", *username);
      fprintf (stderr, "PSK hint: %s\n", hint);
      fprintf (stderr, "PSK key: %s\n", hexkey);
    }

  return 0;
}

static void
init_global_tls_stuff (void)
{
  int ret;

  /* X509 stuff */
  if (gnutls_certificate_allocate_credentials (&xcred) < 0)
    {
      fprintf (stderr, "Certificate allocation memory error\n");
      exit (1);
    }

  if (x509_cafile != NULL)
    {
      ret = gnutls_certificate_set_x509_trust_file (xcred,
                                                    x509_cafile, x509ctype);
    }
  else
    {
      ret = gnutls_certificate_set_x509_system_trust (xcred);
    }
  if (ret < 0)
    {
      fprintf (stderr, "Error setting the x509 trust file\n");
    }
  else
    {
      printf ("Processed %d CA certificate(s).\n", ret);
    }

  if (x509_crlfile != NULL)
    {
      ret = gnutls_certificate_set_x509_crl_file (xcred, x509_crlfile,
                                                  x509ctype);
      if (ret < 0)
        {
          fprintf (stderr, "Error setting the x509 CRL file\n");
        }
      else
        {
          printf ("Processed %d CRL(s).\n", ret);
        }
    }

  load_keys ();

#ifdef ENABLE_OPENPGP
  if (pgp_keyring != NULL)
    {
      ret =
        gnutls_certificate_set_openpgp_keyring_file (xcred, pgp_keyring,
                                                     GNUTLS_OPENPGP_FMT_BASE64);
      if (ret < 0)
        {
          fprintf (stderr, "Error setting the OpenPGP keyring file\n");
        }
    }
#endif

#ifdef ENABLE_SRP
  if (srp_username && srp_passwd)
    {
      /* SRP stuff */
      if (gnutls_srp_allocate_client_credentials (&srp_cred) < 0)
        {
          fprintf (stderr, "SRP authentication error\n");
        }

      gnutls_srp_set_client_credentials_function (srp_cred,
                                                  srp_username_callback);
    }
#endif

#ifdef ENABLE_PSK
  /* PSK stuff */
  if (gnutls_psk_allocate_client_credentials (&psk_cred) < 0)
    {
      fprintf (stderr, "PSK authentication error\n");
    }

  if (psk_username && psk_key.data)
    {
      ret = gnutls_psk_set_client_credentials (psk_cred,
                                               psk_username, &psk_key,
                                               GNUTLS_PSK_KEY_HEX);
      if (ret < 0)
        {
          fprintf (stderr, "Error setting the PSK credentials: %s\n",
                   gnutls_strerror (ret));
        }
    }
  else
    gnutls_psk_set_client_credentials_function (psk_cred, psk_callback);
#endif

#ifdef ENABLE_ANON
  /* ANON stuff */
  if (gnutls_anon_allocate_client_credentials (&anon_cred) < 0)
    {
      fprintf (stderr, "Anonymous authentication error\n");
    }
#endif

}

/* OCSP check for the peer's certificate. Should be called 
 * only after the certificate list verication is complete.
 * Returns:
 * 0: certificate is revoked
 * 1: certificate is ok
 * -1: dunno
 */
static int
cert_verify_ocsp (gnutls_session_t session)
{
  gnutls_x509_crt_t crt, issuer;
  const gnutls_datum_t *cert_list;
  unsigned int cert_list_size = 0;
  int deinit_issuer = 0;
  gnutls_datum_t resp;
  int ret;

  cert_list = gnutls_certificate_get_peers (session, &cert_list_size);
  if (cert_list_size == 0)
    {
      fprintf (stderr, "No certificates found!\n");
      return -1;
    }

  gnutls_x509_crt_init (&crt);
  ret =
      gnutls_x509_crt_import (crt, &cert_list[0],
                              GNUTLS_X509_FMT_DER);
  if (ret < 0)
    {
      fprintf (stderr, "Decoding error: %s\n",
               gnutls_strerror (ret));
      return -1;
    }
    
  ret = gnutls_certificate_get_issuer(xcred, crt, &issuer, 0);
  if (ret < 0 && cert_list_size > 1)
    {
      gnutls_x509_crt_init(&issuer);
      ret = gnutls_x509_crt_import(issuer, &cert_list[1], GNUTLS_X509_FMT_DER);
      if (ret < 0)
        {
          fprintf (stderr, "Decoding error: %s\n",
                   gnutls_strerror (ret));
           return -1;
        }
      deinit_issuer = 1;
    }
  else if (ret < 0)
    {
      fprintf(stderr, "Cannot find issuer\n");
      ret = -1;
      goto cleanup;
    }
    
  ret = send_ocsp_request(NULL, crt, issuer, &resp, 1);
  if (ret < 0)
    {
      fprintf(stderr, "Cannot contact OCSP server\n");
      ret = -1;
      goto cleanup;
    }

  /* verify and check the response for revoked cert */
  ret = check_ocsp_response(issuer, &resp);

cleanup:
  if (deinit_issuer)
    gnutls_x509_crt_deinit (issuer);
  gnutls_x509_crt_deinit (crt);

  return ret;
}
