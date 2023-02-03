/*
** Copyright (C) 2009-2023 Quadrant Information Security <quadrantsec.com>
** Copyright (C) 2009-2023 Champ Clark III <cclark@quadrantsec.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License Version 2 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include "config.h"             /* From autoconf */
#endif

#include <liblognorm.h>
#include <json.h>

#include "normie-defs.h"

int main( int argc, char *argv[])
{

    struct stat liblognorm_fileinfo;
    struct liblognorm_toload_struct *liblognormtoloadstruct;
    int liblognorm_count;
    static ln_ctx ctx;

    int rc_normalize = 0;

    struct json_object *json_norm = NULL;		/* fastjson */
    struct json_object *string_obj = NULL;

    const char *tmp = NULL;

    char *buf = NULL;
    buf = malloc( BUFFER_SIZE );

    if ( buf == NULL )
        {
            fprintf(stderr, "[%s, line %d] Error allocating memory for buffer.\n", __FILE__, __LINE__);
            exit(-1);
        }

    if ( argc < 2 )
        {
            fprintf(stderr, "%s /path/to/normalizations/file.rulebase [optional \"key\" to filter]\n", argv[0]);
            exit(-1);
        }

    /* Load normalization file */

    if((ctx = ln_initCtx()) == NULL)
        {
            fprintf(stderr, "[%s, line %d] Cannot initialize liblognorm context.\n", __FILE__, __LINE__);
            exit(-1);
        }

    if (stat(argv[1], &liblognorm_fileinfo))
        {
            fprintf(stderr, "[%s, line %d] Error accessing '%s'.\n", __FILE__, __LINE__, argv[1]);
            exit(-1);
        }

    ln_loadSamples(ctx, argv[1]);

    while (fgets(buf, BUFFER_SIZE, stdin))
        {

            buf[ strlen(buf) - 1] = '\0';	/* Wipe out the \n */

            rc_normalize = ln_normalize(ctx, buf, strlen(buf), &json_norm);

            if ( json_norm == NULL || rc_normalize != 0 )
                {
                    json_object_put(json_norm);
                    fprintf(stderr, "[%s, line %d] Error processing text\n", __FILE__, __LINE__);
                    exit(-1);
                }

            /* unparsed-data */

            json_object_object_get_ex(json_norm, "unparsed-data", &string_obj);
            tmp = json_object_get_string(string_obj);

            if ( tmp == NULL )
                {
                    /* If only two arguments,  we display the JSON */

                    if ( argc == 2 )
                        {
                            fprintf(stdout, "%s\n", json_object_to_json_string_ext(json_norm, FJSON_TO_STRING_PLAIN));
                        }
                    else
                        {

                            /* If three arguments, then we display the key specified */

                            json_object_object_get_ex(json_norm, argv[2], &string_obj);
                            tmp = json_object_get_string(string_obj);

                            fprintf(stdout, "%s\n", tmp);

                        }
                }
        }

    (void)ln_exitCtx(ctx);
    json_object_put(json_norm);
}
