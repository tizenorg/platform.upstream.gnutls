/*   -*- buffer-read-only: t -*- vi: set ro:
 *
 *  DO NOT EDIT THIS FILE   (genshell.c)
 *
 *  It has been AutoGen-ed  March 31, 2013 at 10:41:27 AM by AutoGen 5.17.3
 *  From the definitions    genshell.def
 *  and the template file   options
 *
 * Generated from AutoOpts 38:0:13 templates.
 *
 *  AutoOpts is a copyrighted work.  This source file is not encumbered
 *  by AutoOpts licensing, but is provided under the licensing terms chosen
 *  by the genshellopt author or copyright holder.  AutoOpts is
 *  licensed under the terms of the LGPL.  The redistributable library
 *  (``libopts'') is licensed under the terms of either the LGPL or, at the
 *  users discretion, the BSD license.  See the AutoOpts and/or libopts sources
 *  for details.
 *
 * The genshellopt program is copyrighted and licensed
 * under the following terms:
 *
 *  Copyright (C) 1999-2013 Bruce Korb, all rights reserved.
 *  This is free software. It is licensed for use, modification and
 *  redistribution under the terms of the GNU Lesser General Public License,
 *  version 2 or later <http://www.gnu.org/licenses/old-licenses/lgpl-2.0.html>
 *
 *  The genshellopt library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, see
 *  <http://www.gnu.org/licenses/old-licenses/lgpl-2.0.html>
 */

#ifndef __doxygen__
#define OPTION_CODE_COMPILE 1
#include "genshell.h"
#include <sys/types.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef  __cplusplus
extern "C" {
#endif
extern FILE * option_usage_fp;
#define zCopyright      (genshellopt_opt_strs+0)
#define zLicenseDescrip (genshellopt_opt_strs+285)

extern tUsageProc genshelloptUsage;

#ifndef NULL
#  define NULL 0
#endif

/**
 *  static const strings for genshellopt options
 */
static char const genshellopt_opt_strs[1769] =
/*     0 */ "genshellopt 1\n"
            "Copyright (C) 1999-2013 Bruce Korb, all rights reserved.\n"
            "This is free software. It is licensed for use, modification and\n"
            "redistribution under the terms of the GNU Lesser General Public License,\n"
            "version 2 or later <http://www.gnu.org/licenses/old-licenses/lgpl-2.0.html>\n\0"
/*   285 */ "The genshellopt library is free software; you can redistribute it and/or\n"
            "modify it under the terms of the GNU Library General Public License as\n"
            "published by the Free Software Foundation; either version 2 of the License,\n"
            "or (at your option) any later version.\n\n"
            "This library is distributed in the hope that it will be useful, but WITHOUT\n"
            "ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or\n"
            "FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public\n"
            "License for more details.\n\n"
            "You should have received a copy of the GNU Library General Public License\n"
            "along with this library; if not, see\n"
            "<http://www.gnu.org/licenses/old-licenses/lgpl-2.0.html>\n\0"
/*   957 */ "Output Script File\0"
/*   976 */ "SCRIPT\0"
/*   983 */ "script\0"
/*   990 */ "Shell name (follows \"#!\" magic)\0"
/*  1022 */ "SHELL\0"
/*  1028 */ "no-shell\0"
/*  1037 */ "no\0"
/*  1040 */ "display extended usage information and exit\0"
/*  1084 */ "help\0"
/*  1089 */ "extended usage information passed thru pager\0"
/*  1134 */ "more-help\0"
/*  1144 */ "output version information and exit\0"
/*  1180 */ "version\0"
/*  1188 */ "GENSHELLOPT\0"
/*  1200 */ "genshellopt - Generate Shell Option Processing Script - Ver. 1\n"
            "Usage:  %s [ -<flag> [<val>] | --<name>[{=| }<val>] ]...\n\0"
/*  1321 */ "autogen-users@lists.sourceforge.net\0"
/*  1357 */ "Note that 'shell' is only useful if the output file does not already exist.\n"
            "If it does, then the shell name and optional first argument will be\n"
            "extracted from the script file.\n\0"
/*  1534 */ "If the script file already exists and contains Automated Option Processing\n"
            "text, the second line of the file through the ending tag will be replaced\n"
            "by the newly generated text.  The first '#!' line will be regenerated.\n\0"
/*  1755 */ "genshellopt 1";

/**
 *  script option description:
 */
/** Descriptive text for the script option */
#define SCRIPT_DESC      (genshellopt_opt_strs+957)
/** Upper-cased name for the script option */
#define SCRIPT_NAME      (genshellopt_opt_strs+976)
/** Name string for the script option */
#define SCRIPT_name      (genshellopt_opt_strs+983)
/** Compiled in flag settings for the script option */
#define SCRIPT_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

/**
 *  shell option description:
 */
/** Descriptive text for the shell option */
#define SHELL_DESC      (genshellopt_opt_strs+990)
/** Upper-cased name for the shell option */
#define SHELL_NAME      (genshellopt_opt_strs+1022)
/** disablement name for the shell option */
#define NOT_SHELL_name  (genshellopt_opt_strs+1028)
/** disablement prefix for the shell option */
#define NOT_SHELL_PFX   (genshellopt_opt_strs+1037)
/** Name string for the shell option */
#define SHELL_name      (NOT_SHELL_name + 3)
/** Compiled in flag settings for the shell option */
#define SHELL_FLAGS     (OPTST_INITENABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

/*
 *  Help/More_Help/Version option descriptions:
 */
#define HELP_DESC       (genshellopt_opt_strs+1040)
#define HELP_name       (genshellopt_opt_strs+1084)
#ifdef HAVE_WORKING_FORK
#define MORE_HELP_DESC  (genshellopt_opt_strs+1089)
#define MORE_HELP_name  (genshellopt_opt_strs+1134)
#define MORE_HELP_FLAGS (OPTST_IMM | OPTST_NO_INIT)
#else
#define MORE_HELP_DESC  NULL
#define MORE_HELP_name  NULL
#define MORE_HELP_FLAGS (OPTST_OMITTED | OPTST_NO_INIT)
#endif
#ifdef NO_OPTIONAL_OPT_ARGS
#  define VER_FLAGS     (OPTST_IMM | OPTST_NO_INIT)
#else
#  define VER_FLAGS     (OPTST_SET_ARGTYPE(OPARG_TYPE_STRING) | \
                         OPTST_ARG_OPTIONAL | OPTST_IMM | OPTST_NO_INIT)
#endif
#define VER_DESC        (genshellopt_opt_strs+1144)
#define VER_name        (genshellopt_opt_strs+1180)
/**
 *  Declare option callback procedures
 */
extern tOptProc
    optionBooleanVal,   optionNestedVal,    optionNumericVal,
    optionPagedUsage,   optionPrintVersion, optionResetOpt,
    optionStackArg,     optionTimeDate,     optionTimeVal,
    optionUnstackArg,   optionVendorOption;
static tOptProc
    doUsageOpt;
#define VER_PROC        optionPrintVersion

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 *  Define the genshellopt Option Descriptions.
 * This is an array of GENSHELL_OPTION_CT entries, one for each
 * option that the genshellopt program responds to.
 */
static tOptDesc optDesc[GENSHELL_OPTION_CT] = {
  {  /* entry idx, value */ 0, VALUE_GENSHELL_OPT_SCRIPT,
     /* equiv idx, value */ 0, VALUE_GENSHELL_OPT_SCRIPT,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ SCRIPT_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --script */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ SCRIPT_DESC, SCRIPT_NAME, SCRIPT_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 1, VALUE_GENSHELL_OPT_SHELL,
     /* equiv idx, value */ 1, VALUE_GENSHELL_OPT_SHELL,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ SHELL_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --shell */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ SHELL_DESC, SHELL_NAME, SHELL_name,
     /* disablement strs */ NOT_SHELL_name, NOT_SHELL_PFX },

  {  /* entry idx, value */ INDEX_GENSHELL_OPT_VERSION, VALUE_GENSHELL_OPT_VERSION,
     /* equiv idx value  */ NO_EQUIVALENT, VALUE_GENSHELL_OPT_VERSION,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ VER_FLAGS, AOUSE_VERSION,
     /* last opt argumnt */ { NULL },
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ VER_PROC,
     /* desc, NAME, name */ VER_DESC, NULL, VER_name,
     /* disablement strs */ NULL, NULL },



  {  /* entry idx, value */ INDEX_GENSHELL_OPT_HELP, VALUE_GENSHELL_OPT_HELP,
     /* equiv idx value  */ NO_EQUIVALENT, VALUE_GENSHELL_OPT_HELP,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ OPTST_IMM | OPTST_NO_INIT, AOUSE_HELP,
     /* last opt argumnt */ { NULL },
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ doUsageOpt,
     /* desc, NAME, name */ HELP_DESC, NULL, HELP_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ INDEX_GENSHELL_OPT_MORE_HELP, VALUE_GENSHELL_OPT_MORE_HELP,
     /* equiv idx value  */ NO_EQUIVALENT, VALUE_GENSHELL_OPT_MORE_HELP,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ MORE_HELP_FLAGS, AOUSE_MORE_HELP,
     /* last opt argumnt */ { NULL },
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL,  NULL,
     /* option proc      */ optionPagedUsage,
     /* desc, NAME, name */ MORE_HELP_DESC, NULL, MORE_HELP_name,
     /* disablement strs */ NULL, NULL }
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/** Reference to the upper cased version of genshellopt. */
#define zPROGNAME       (genshellopt_opt_strs+1188)
/** Reference to the title line for genshellopt usage. */
#define zUsageTitle     (genshellopt_opt_strs+1200)
/** There is no genshellopt configuration file. */
#define zRcName         NULL
/** There are no directories to search for genshellopt config files. */
#define apzHomeList     NULL
/** The genshellopt program bug email address. */
#define zBugsAddr       (genshellopt_opt_strs+1321)
/** Clarification/explanation of what genshellopt does. */
#define zExplain        (genshellopt_opt_strs+1357)
/** Extra detail explaining what genshellopt does. */
#define zDetail         (genshellopt_opt_strs+1534)
/** The full version string for genshellopt. */
#define zFullVersion    (genshellopt_opt_strs+1755)
/* extracted from optcode.tlib near line 371 */

#if defined(ENABLE_NLS)
# define OPTPROC_BASE OPTPROC_TRANSLATE
  static tOptionXlateProc translate_option_strings;
#else
# define OPTPROC_BASE OPTPROC_NONE
# define translate_option_strings NULL
#endif /* ENABLE_NLS */

#define genshellopt_full_usage (NULL)
#define genshellopt_short_usage (NULL)

#endif /* not defined __doxygen__ */

/*
 *  Create the static procedure(s) declared above.
 */
/**
 * The callout function that invokes the genshelloptUsage function.
 *
 * @param[in] pOptions the AutoOpts option description structure
 * @param[in] pOptDesc the descriptor for the "help" (usage) option.
 * @noreturn
 */
static void
doUsageOpt(tOptions * pOptions, tOptDesc * pOptDesc)
{
    genshelloptUsage(&genshelloptOptions, GENSHELLOPT_EXIT_SUCCESS);
    /* NOTREACHED */
    (void)pOptDesc;
    (void)pOptions;
}
/* extracted from optmain.tlib near line 1254 */

/**
 * The directory containing the data associated with genshellopt.
 */
#ifndef  PKGDATADIR
# define PKGDATADIR ""
#endif

/**
 * Information about the person or institution that packaged genshellopt
 * for the current distribution.
 */
#ifndef  WITH_PACKAGER
# define genshellopt_packager_info NULL
#else
/** Packager information for genshellopt. */
static char const genshellopt_packager_info[] =
    "Packaged by " WITH_PACKAGER

# ifdef WITH_PACKAGER_VERSION
        " ("WITH_PACKAGER_VERSION")"
# endif

# ifdef WITH_PACKAGER_BUG_REPORTS
    "\nReport genshellopt bugs to " WITH_PACKAGER_BUG_REPORTS
# endif
    "\n";
#endif
#ifndef __doxygen__

#endif /* __doxygen__ */
/**
 * The option definitions for genshellopt.  The one structure that
 * binds them all.
 */
tOptions genshelloptOptions = {
    OPTIONS_STRUCT_VERSION,
    0, NULL,                    /* original argc + argv    */
    ( OPTPROC_BASE
    + OPTPROC_ERRSTOP
    + OPTPROC_SHORTOPT
    + OPTPROC_LONGOPT
    + OPTPROC_NO_REQ_OPT
    + OPTPROC_NEGATIONS
    + OPTPROC_NO_ARGS ),
    0, NULL,                    /* current option index, current option */
    NULL,         NULL,         zPROGNAME,
    zRcName,      zCopyright,   zLicenseDescrip,
    zFullVersion, apzHomeList,  zUsageTitle,
    zExplain,     zDetail,      optDesc,
    zBugsAddr,                  /* address to send bugs to */
    NULL, NULL,                 /* extensions/saved state  */
    genshelloptUsage, /* usage procedure */
    translate_option_strings,   /* translation procedure */
    /*
     *  Indexes to special options
     */
    { INDEX_GENSHELL_OPT_MORE_HELP, /* more-help option index */
      NO_EQUIVALENT, /* save option index */
      NO_EQUIVALENT, /* '-#' option index */
      NO_EQUIVALENT /* index of default opt */
    },
    5 /* full option count */, 2 /* user option count */,
    genshellopt_full_usage, genshellopt_short_usage,
    NULL, NULL,
    PKGDATADIR, genshellopt_packager_info
};

#if ENABLE_NLS
/**
 * This code is designed to translate translatable option text for the
 * genshellopt program.  These translations happen upon entry
 * to optionProcess().
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef HAVE_DCGETTEXT
# include <gettext.h>
#endif
#include <autoopts/usage-txt.h>

static char * AO_gettext(char const * pz);
static void   coerce_it(void ** s);

/**
 * AutoGen specific wrapper function for gettext.  It relies on the macro _()
 * to convert from English to the target language, then strdup-duplicates the
 * result string.  It tries the "libopts" domain first, then whatever has been
 * set via the \a textdomain(3) call.
 *
 * @param[in] pz the input text used as a lookup key.
 * @returns the translated text (if there is one),
 *   or the original text (if not).
 */
static char *
AO_gettext(char const * pz)
{
    char * res;
    if (pz == NULL)
        return NULL;
#ifdef HAVE_DCGETTEXT
    /*
     * While processing the option_xlateable_txt data, try to use the
     * "libopts" domain.  Once we switch to the option descriptor data,
     * do *not* use that domain.
     */
    if (option_xlateable_txt.field_ct != 0) {
        res = dgettext("libopts", pz);
        if (res == pz)
            res = (char *)(void *)_(pz);
    } else
        res = (char *)(void *)_(pz);
#else
    res = (char *)(void *)_(pz);
#endif
    if (res == pz)
        return res;
    res = strdup(res);
    if (res == NULL) {
        fputs(_("No memory for duping translated strings\n"), stderr);
        exit(GENSHELLOPT_EXIT_FAILURE);
    }
    return res;
}

/**
 * All the pointers we use are marked "* const", but they are stored in
 * writable memory.  Coerce the mutability and set the pointer.
 */
static void coerce_it(void ** s) { *s = AO_gettext(*s);
}

/**
 * Translate all the translatable strings in the genshelloptOptions
 * structure defined above.  This is done only once.
 */
static void
translate_option_strings(void)
{
    tOptions * const opts = &genshelloptOptions;

    /*
     *  Guard against re-translation.  It won't work.  The strings will have
     *  been changed by the first pass through this code.  One shot only.
     */
    if (option_xlateable_txt.field_ct != 0) {
        /*
         *  Do the translations.  The first pointer follows the field count
         *  field.  The field count field is the size of a pointer.
         */
        char ** ppz = (char**)(void*)&(option_xlateable_txt);
        int     ix  = option_xlateable_txt.field_ct;

        do {
            ppz++; /* skip over field_ct */
            *ppz = AO_gettext(*ppz);
        } while (--ix > 0);
        /* prevent re-translation and disable "libopts" domain lookup */
        option_xlateable_txt.field_ct = 0;

        coerce_it((void*)&(opts->pzCopyright));
        coerce_it((void*)&(opts->pzCopyNotice));
        coerce_it((void*)&(opts->pzFullVersion));
        coerce_it((void*)&(opts->pzUsageTitle));
        coerce_it((void*)&(opts->pzExplain));
        coerce_it((void*)&(opts->pzDetail));
        {
            tOptDesc * od = opts->pOptDesc;
            for (ix = opts->optCt; ix > 0; ix--, od++)
                coerce_it((void*)&(od->pzText));
        }
    }
}
#endif /* ENABLE_NLS */

#ifdef DO_NOT_COMPILE_THIS_CODE_IT_IS_FOR_GETTEXT
/** I18N function strictly for xgettext.  Do not compile. */
static void bogus_function(void) {
  /* TRANSLATORS:

     The following dummy function was crated solely so that xgettext can extract
     the correct strings.  These strings are actually referenced by a field name
     in the genshelloptOptions structure noted in the comments below.  The
     literal text is defined in genshellopt_opt_strs.
   
     NOTE: the strings below are segmented with respect to the source string
     genshellopt_opt_strs.  The strings above are handed off for translation
     at run time a paragraph at a time.  Consequently, they are presented here
     for translation a paragraph at a time.
   
     ALSO: often the description for an option will reference another option
     by name.  These are set off with apostrophe quotes (I hope).  Do not
     translate option names.
   */
  /* referenced via genshelloptOptions.pzCopyright */
  puts(_("genshellopt 1\n\
Copyright (C) 1999-2013 Bruce Korb, all rights reserved.\n\
This is free software. It is licensed for use, modification and\n\
redistribution under the terms of the GNU Lesser General Public License,\n\
version 2 or later <http://www.gnu.org/licenses/old-licenses/lgpl-2.0.html>\n"));

  /* referenced via genshelloptOptions.pzCopyNotice */
  puts(_("The genshellopt library is free software; you can redistribute it and/or\n\
modify it under the terms of the GNU Library General Public License as\n\
published by the Free Software Foundation; either version 2 of the License,\n\
or (at your option) any later version.\n\n"));
  puts(_("This library is distributed in the hope that it will be useful, but WITHOUT\n\
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or\n\
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public\n\
License for more details.\n\n"));
  puts(_("You should have received a copy of the GNU Library General Public License\n\
along with this library; if not, see\n\
<http://www.gnu.org/licenses/old-licenses/lgpl-2.0.html>\n"));

  /* referenced via genshelloptOptions.pOptDesc->pzText */
  puts(_("Output Script File"));

  /* referenced via genshelloptOptions.pOptDesc->pzText */
  puts(_("Shell name (follows \"#!\" magic)"));

  /* referenced via genshelloptOptions.pOptDesc->pzText */
  puts(_("display extended usage information and exit"));

  /* referenced via genshelloptOptions.pOptDesc->pzText */
  puts(_("extended usage information passed thru pager"));

  /* referenced via genshelloptOptions.pOptDesc->pzText */
  puts(_("output version information and exit"));

  /* referenced via genshelloptOptions.pzUsageTitle */
  puts(_("genshellopt - Generate Shell Option Processing Script - Ver. 1\n\
Usage:  %s [ -<flag> [<val>] | --<name>[{=| }<val>] ]...\n"));

  /* referenced via genshelloptOptions.pzExplain */
  puts(_("Note that 'shell' is only useful if the output file does not already exist.\n\
If it does, then the shell name and optional first argument will be\n\
extracted from the script file.\n"));

  /* referenced via genshelloptOptions.pzDetail */
  puts(_("If the script file already exists and contains Automated Option Processing\n\
text, the second line of the file through the ending tag will be replaced\n\
by the newly generated text.  The first '#!' line will be regenerated.\n"));

  /* referenced via genshelloptOptions.pzFullVersion */
  puts(_("genshellopt 1"));

  /* referenced via genshelloptOptions.pzFullUsage */
  puts(_("<<<NOT-FOUND>>>"));

  /* referenced via genshelloptOptions.pzShortUsage */
  puts(_("<<<NOT-FOUND>>>"));
  /* LIBOPTS-MESSAGES: */
#line 60 "../autoopts.c"
  puts(_("allocation of %d bytes failed\n"));
#line 86 "../autoopts.c"
  puts(_("allocation of %d bytes failed\n"));
#line 53 "../init.c"
  puts(_("AutoOpts function called without option descriptor\n"));
#line 89 "../init.c"
  puts(_("\tThis exceeds the compiled library version:  "));
#line 87 "../init.c"
  puts(_("Automated Options Processing Error!\n"
       "\t%s called AutoOpts function with structure version %d:%d:%d.\n"));
#line 73 "../autoopts.c"
  puts(_("realloc of %d bytes at 0x%p failed\n"));
#line 91 "../init.c"
  puts(_("\tThis is less than the minimum library version:  "));
#line 121 "../version.c"
  puts(_("Automated Options version %s\n"
       "\tCopyright (C) 1999-2013 by Bruce Korb - all rights reserved\n"));
#line 310 "../usage.c"
  puts(_("(AutoOpts bug):  %s.\n"));
#line 90 "../reset.c"
  puts(_("optionResetOpt() called, but reset-option not configured"));
#line 329 "../usage.c"
  puts(_("could not locate the 'help' option"));
#line 351 "../autoopts.c"
  puts(_("optionProcess() was called with invalid data"));
#line 783 "../usage.c"
  puts(_("invalid argument type specified"));
#line 589 "../find.c"
  puts(_("defaulted to option with optional arg"));
#line 76 "../alias.c"
  puts(_("aliasing option is out of range."));
#line 229 "../enum.c"
  puts(_("%s error:  the keyword '%s' is ambiguous for %s\n"));
#line 108 "../find.c"
  puts(_("  The following options match:\n"));
#line 290 "../find.c"
  puts(_("%s: ambiguous option name: %s (matches %d options)\n"));
#line 161 "../check.c"
  puts(_("%s: Command line arguments required\n"));
#line 43 "../alias.c"
  puts(_("%d %s%s options allowed\n"));
#line 81 "../makeshell.c"
  puts(_("%s error %d (%s) calling %s for '%s'\n"));
#line 293 "../makeshell.c"
  puts(_("interprocess pipe"));
#line 168 "../version.c"
  puts(_("error: version option argument '%c' invalid.  Use:\n"
       "\t'v' - version only\n"
       "\t'c' - version and copyright\n"
       "\t'n' - version and full copyright notice\n"));
#line 58 "../check.c"
  puts(_("%s error:  the '%s' and '%s' options conflict\n"));
#line 214 "../find.c"
  puts(_("%s: The '%s' option has been disabled."));
#line 421 "../find.c"
  puts(_("%s: The '%s' option has been disabled."));
#line 38 "../alias.c"
  puts(_("-equivalence"));
#line 460 "../find.c"
  puts(_("%s: illegal option -- %c\n"));
#line 110 "../reset.c"
  puts(_("%s: illegal option -- %c\n"));
#line 268 "../find.c"
  puts(_("%s: illegal option -- %s\n"));
#line 746 "../find.c"
  puts(_("%s: illegal option -- %s\n"));
#line 118 "../reset.c"
  puts(_("%s: illegal option -- %s\n"));
#line 332 "../find.c"
  puts(_("%s: unknown vendor extension option -- %s\n"));
#line 154 "../enum.c"
  puts(_("  or an integer from %d through %d\n"));
#line 164 "../enum.c"
  puts(_("  or an integer from %d through %d\n"));
#line 782 "../usage.c"
  puts(_("%s error:  invalid option descriptor for %s\n"));
#line 1110 "../usage.c"
  puts(_("%s error:  invalid option descriptor for %s\n"));
#line 379 "../find.c"
  puts(_("%s: invalid option name: %s\n"));
#line 518 "../find.c"
  puts(_("%s: The '%s' option requires an argument.\n"));
#line 171 "../autoopts.c"
  puts(_("(AutoOpts bug):  Equivalenced option '%s' was equivalenced to both\n"
       "\t'%s' and '%s'."));
#line 94 "../check.c"
  puts(_("%s error:  The %s option is required\n"));
#line 623 "../find.c"
  puts(_("%s: The '%s' option cannot have an argument.\n"));
#line 151 "../check.c"
  puts(_("%s: Command line arguments are not allowed.\n"));
#line 531 "../save.c"
  puts(_("error %d (%s) creating %s\n"));
#line 229 "../enum.c"
  puts(_("%s error:  '%s' does not match any %s keywords.\n"));
#line 93 "../reset.c"
  puts(_("%s error: The '%s' option requires an argument.\n"));
#line 184 "../save.c"
  puts(_("error %d (%s) stat-ing %s\n"));
#line 238 "../save.c"
  puts(_("error %d (%s) stat-ing %s\n"));
#line 143 "../restore.c"
  puts(_("%s error: no saved option state\n"));
#line 246 "../autoopts.c"
  puts(_("'%s' is not a command line option.\n"));
#line 114 "../time.c"
  puts(_("%s error:  '%s' is not a recognizable date/time.\n"));
#line 132 "../save.c"
  puts(_("'%s' not defined\n"));
#line 53 "../time.c"
  puts(_("%s error:  '%s' is not a recognizable time duration.\n"));
#line 92 "../check.c"
  puts(_("%s error:  The %s option must appear %d times.\n"));
#line 157 "../numeric.c"
  puts(_("%s error:  '%s' is not a recognizable number.\n"));
#line 195 "../enum.c"
  puts(_("%s error:  %s exceeds %s keyword count\n"));
#line 366 "../usage.c"
  puts(_("Try '%s %s' for more information.\n"));
#line 45 "../alias.c"
  puts(_("one %s%s option allowed\n"));
#line 195 "../makeshell.c"
  puts(_("standard output"));
#line 930 "../makeshell.c"
  puts(_("standard output"));
#line 304 "../usage.c"
  puts(_("standard output"));
#line 451 "../usage.c"
  puts(_("standard output"));
#line 660 "../usage.c"
  puts(_("standard output"));
#line 175 "../version.c"
  puts(_("standard output"));
#line 304 "../usage.c"
  puts(_("standard error"));
#line 451 "../usage.c"
  puts(_("standard error"));
#line 660 "../usage.c"
  puts(_("standard error"));
#line 175 "../version.c"
  puts(_("standard error"));
#line 195 "../makeshell.c"
  puts(_("write"));
#line 930 "../makeshell.c"
  puts(_("write"));
#line 303 "../usage.c"
  puts(_("write"));
#line 450 "../usage.c"
  puts(_("write"));
#line 659 "../usage.c"
  puts(_("write"));
#line 174 "../version.c"
  puts(_("write"));
#line 60 "../numeric.c"
  puts(_("%s error:  %s option value %ld is out of range.\n"));
#line 44 "../check.c"
  puts(_("%s error:  %s option requires the %s option\n"));
#line 131 "../save.c"
  puts(_("%s warning:  cannot save options - %s not regular file\n"));
#line 183 "../save.c"
  puts(_("%s warning:  cannot save options - %s not regular file\n"));
#line 237 "../save.c"
  puts(_("%s warning:  cannot save options - %s not regular file\n"));
#line 256 "../save.c"
  puts(_("%s warning:  cannot save options - %s not regular file\n"));
#line 530 "../save.c"
  puts(_("%s warning:  cannot save options - %s not regular file\n"));
  /* END-LIBOPTS-MESSAGES */

  /* USAGE-TEXT: */
#line 908 "../usage.c"
  puts(_("\t\t\t\t- an alternate for '%s'\n"));
#line 1177 "../usage.c"
  puts(_("Version, usage and configuration options:"));
#line 959 "../usage.c"
  puts(_("\t\t\t\t- default option for unnamed options\n"));
#line 872 "../usage.c"
  puts(_("\t\t\t\t- disabled as '--%s'\n"));
#line 1146 "../usage.c"
  puts(_(" --- %-14s %s\n"));
#line 1144 "../usage.c"
  puts(_("This option has been disabled"));
#line 899 "../usage.c"
  puts(_("\t\t\t\t- enabled by default\n"));
#line 40 "../alias.c"
  puts(_("%s error:  only "));
#line 1221 "../usage.c"
  puts(_(" - examining environment variables named %s_*\n"));
#line 168 "../file.c"
  puts(_("\t\t\t\t- file must not pre-exist\n"));
#line 172 "../file.c"
  puts(_("\t\t\t\t- file must pre-exist\n"));
#line 416 "../usage.c"
  puts(_("Options are specified by doubled hyphens and their name or by a single\n"
       "hyphen and the flag character.\n"));
#line 908 "../makeshell.c"
  puts(_("\n"
       "= = = = = = = =\n\n"
       "This incarnation of genshell will produce\n"
       "a shell script to parse the options for %s:\n\n"));
#line 161 "../enum.c"
  puts(_("  or an integer mask with any of the lower %d bits set\n"));
#line 932 "../usage.c"
  puts(_("\t\t\t\t- is a set membership option\n"));
#line 953 "../usage.c"
  puts(_("\t\t\t\t- must appear between %d and %d times\n"));
#line 418 "../usage.c"
  puts(_("Options are specified by single or double hyphens and their name.\n"));
#line 939 "../usage.c"
  puts(_("\t\t\t\t- may appear multiple times\n"));
#line 926 "../usage.c"
  puts(_("\t\t\t\t- may not be preset\n"));
#line 1336 "../usage.c"
  puts(_("   Arg Option-Name    Description\n"));
#line 1272 "../usage.c"
  puts(_("  Flg Arg Option-Name    Description\n"));
#line 1330 "../usage.c"
  puts(_("  Flg Arg Option-Name    Description\n"));
#line 1331 "../usage.c"
  puts(_(" %3s %s"));
#line 1337 "../usage.c"
  puts(_(" %3s %s"));
#line 423 "../usage.c"
  puts(_("The '-#<number>' option may omit the hash char\n"));
#line 419 "../usage.c"
  puts(_("All arguments are named options.\n"));
#line 1006 "../usage.c"
  puts(_(" - reading file %s"));
#line 445 "../usage.c"
  puts(_("\n"
       "Please send bug reports to:  <%s>\n"));
#line 100 "../version.c"
  puts(_("\n"
       "Please send bug reports to:  <%s>\n"));
#line 129 "../version.c"
  puts(_("\n"
       "Please send bug reports to:  <%s>\n"));
#line 938 "../usage.c"
  puts(_("\t\t\t\t- may NOT appear - preset only\n"));
#line 978 "../usage.c"
  puts(_("\n"
       "The following option preset mechanisms are supported:\n"));
#line 1219 "../usage.c"
  puts(_("\n"
       "The following option preset mechanisms are supported:\n"));
#line 717 "../usage.c"
  puts(_("prohibits these options:\n"));
#line 712 "../usage.c"
  puts(_("prohibits the option '%s'\n"));
#line 81 "../numeric.c"
  puts(_("%s%ld to %ld"));
#line 79 "../numeric.c"
  puts(_("%sgreater than or equal to %ld"));
#line 75 "../numeric.c"
  puts(_("%s%ld exactly"));
#line 68 "../numeric.c"
  puts(_("%sit must lie in one of the ranges:\n"));
#line 68 "../numeric.c"
  puts(_("%sit must be in the range:\n"));
#line 88 "../numeric.c"
  puts(_(", or\n"));
#line 66 "../numeric.c"
  puts(_("%sis scalable with a suffix: k/K/m/M/g/G/t/T\n"));
#line 77 "../numeric.c"
  puts(_("%sless than or equal to %ld"));
#line 426 "../usage.c"
  puts(_("Operands and options may be intermixed.  They will be reordered.\n"));
#line 687 "../usage.c"
  puts(_("requires the option '%s'\n"));
#line 690 "../usage.c"
  puts(_("requires these options:\n"));
#line 1348 "../usage.c"
  puts(_("   Arg Option-Name   Req?  Description\n"));
#line 1342 "../usage.c"
  puts(_("  Flg Arg Option-Name   Req?  Description\n"));
#line 162 "../enum.c"
  puts(_("or you may use a numeric representation.  Preceding these with a '!'\n"
       "will clear the bits, specifying 'none' will clear all bits, and 'all'\n"
       "will set them all.  Multiple entries may be passed as an option\n"
       "argument list.\n"));
#line 945 "../usage.c"
  puts(_("\t\t\t\t- may appear up to %d times\n"));
#line 72 "../enum.c"
  puts(_("The valid \"%s\" option keywords are:\n"));
#line 1181 "../usage.c"
  puts(_("The next option supports vendor supported extra options:"));
#line 808 "../usage.c"
  puts(_("These additional options are:"));
  /* END-USAGE-TEXT */
}
#endif /* uncompilable code */
#ifdef  __cplusplus
}
#endif
/* genshell.c ends here */
