/*
 * Copyright (C) 2000-2012 Free Software Foundation, Inc.
 *
 * Author: Nikos Mavrogiannopoulos
 *
 * This file is part of GnuTLS.
 *
 * The GnuTLS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

#ifndef GNUTLS_DATUM_H
#define GNUTLS_DATUM_H

void _gnutls_write_datum16 (uint8_t * dest, gnutls_datum_t dat);
void _gnutls_write_datum24 (uint8_t * dest, gnutls_datum_t dat);
void _gnutls_write_datum32 (uint8_t * dest, gnutls_datum_t dat);
void _gnutls_write_datum8 (uint8_t * dest, gnutls_datum_t dat);

int _gnutls_set_datum (gnutls_datum_t * dat, const void *data,
                         size_t data_size);

int _gnutls_datum_append (gnutls_datum_t * dat, const void *data,
                            size_t data_size);

void _gnutls_free_datum (gnutls_datum_t * dat);

#endif
