// Make court cards

#include <stdio.h>
#include <string.h>
#include <popt.h>
#include <time.h>
#include <sys/time.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <err.h>
#include <signal.h>
#include <execinfo.h>
#include <axl.h>

const char *colour[] = { "Black", "Blue", "Red", "Gold", "Stroke", "Thin" };

const char *card[] = { "JS", "QS", "KS", "JH", "QH", "KH", "JC", "QC", "KC", "JD", "QD", "KD",
   "1J", "2J", "AS", "1B"
};

struct
{
   int x,
     y,
     s,
     r,
     border;
} pips[sizeof (card) / sizeof (*card)][20] = {
   {
    {
     910, 1454, 75, 35}
    ,
    {
     845, 1359, 75, 33}
    ,
    {
     780, 1261, 75, 33}
    }
   ,                            // JS
   {
    {
     1188, 1065, 90, -40}
    ,
    {
     1194, 957, 90, -40}
    ,
    {
     1096, 967, 90, -40}
    ,
    {
     1022, 1053, 90, -40}
    ,
    {
     918, 1149, 90, -40}
    ,
    {
     897, 1274, 90, -40}
    }
   ,                            // QS
   {
    {
     495, 1388, 90, -10}
    ,
    {
     528, 1249, 90, -5}
    ,
    {
     839, 1360, 90, 20}
    ,
    {
     795, 1251, 90, 20}
    }
   ,                            // KS
   {
    {
     546, 1000, 100, 0}
    ,
    {
     615, 1000, 100, 180}
    ,
    {
     883, 1224, 75, 30}
    ,
    {
     915, 1289, 75, 30}
    ,
    {
     954, 1354, 75, 30}
    ,
    {
     220, 1367, 75, 60}
    ,
    {
     155, 1272, 75, 60}
    ,
    {
     52, 1272, 75, 60}
    ,
    {
     96, 1175, 75, 60}
    ,}
   ,                            // JH
   {
    {
     967, 812, 75, 30}
    ,
    {
     1013, 933, 75, 15}
    ,
    {
     1041, 1058, 75, 3}
    ,
    {
     1054, 1184, 75, 5}
    ,
    {
     1068, 1301, 75, 10}
    }
   ,                            // QH
   {
    {
     988, 1199, 90, 85}
    ,
    {
     958, 1298, 90, 60}
    ,
    {
     975, 1089, 90, 110}
    ,
    {
     922, 997, 90, 130}
    }
   ,                            // KH
   {
    {
     504, 1272, 75, 0}
    ,
    {
     504, 1156, 75, 0}
    ,
    {
     826, 1357, 75, 0}
    }
   ,                            // JC
   {
    {
     1017, 1099, 90, -45}
    ,
    {
     1125, 1022, 90, -45}
    ,
    {
     1229, 940, 90, -45}
    }
   ,                            // QC
   {
    {
     893, 1141, 90, 5}
    ,
    {
     927, 1279, 90, 15}
    ,
    {
     992, 1399, 90, 25}
    }
   ,                            // KC
   {
    {
     650, 1000, 200, 0, 1}
    ,
    {
     894, 1402, 50, -26}
    ,
    {
     852, 1377, 50, -22}
    ,
    {
     806, 1361, 50, -14}
    ,
    {
     757, 1350, 50, -6}
    ,
    {
     706, 1345, 50, 0}
    ,
    {
     656, 1347, 50, 5}
    ,
    {
     607, 1353, 50, 10}
    ,
    {
     560, 1363, 50, 15}
    ,}
   ,                            // JD
   {
    {
     650, 1000, 100, 0, 1}
    ,
    {
     580, 995, 100, 0, 1}
    ,
    {
     510, 990, 100, 0, 1}
    }
   ,                            // QD
   {
    {
     650, 1000, 120, 0}
    ,
    {
     570, 1000, 120, 0}
    ,
    {
     400, 1360, 170, -15, 1}
    ,
    {
     435, 1235, 150, -10, 1}
    ,
    {
     453, 1115, 130, -5, 1}
    ,
    {
     460, 1018, 110, 0, 1}
    ,
    {
     450, 930, 100, 10, 1}
    ,
    {
     1259, 1000, 90, 60}
    ,
    {
     1173, 948, 90, 50}
    ,
    {
     1094, 880, 90, 40}
    ,
    {
     1038, 791, 90, 30}
    }
   ,                            // KD
   {
    }
   ,                            // AS
   {
    }
   ,                            // JOKER
   {
    }
   ,                            // IGNIS
};

int
main (int argc, const char *argv[])
{
   xml_t cardxml[sizeof (card) / sizeof (*card)] = { };
   FILE *f = fopen ("court.h", "w");
   if (!f)
      err (1, "Cannot make court.h");
   int n,
     c;
   for (c = 0; c < sizeof (card) / sizeof (*card); c++)
   {
      char *fn;
      if (!asprintf (&fn, "../svg/%s.svg", card[c]))
         errx (1, "malloc");
      cardxml[c] = xml_tree_read_file (fn);
      free (fn);
   }
   for (n = 0; n < sizeof (colour) / sizeof (*colour); n++)
   {                            // New court card logic
      fprintf (f, "const char *%s_path[%d]={\n", colour[n], (int) (sizeof (card) / sizeof (*card)));
      for (c = 0; c < sizeof (card) / sizeof (*card); c++)
      {
         char *d = NULL;
         if (cardxml[c])
         {
            xml_t g = NULL;
            while ((g = xml_element_next_by_name (cardxml[c], g, "g")))
               if (!strcasecmp (xml_get (g, "@inkscape:label") ? : "", colour[n]))
                  break;
            if (!g)
               warnx ("Cannot find %s in %s", colour[n], card[c]);
            else
            {
               size_t len;
               FILE *path = open_memstream (&d, &len);
               xml_t p = NULL;
               while ((p = xml_element_next_by_name (g, p, "path")))
               {
                  if (xml_get (p, "@transform"))
                     errx (1, "Transform found on path in %s in %s", colour[n], card[c]);
                  d = xml_get (p, "@d");
                  if (d)
                  {
                     if (*d == 'm' && d[1] == ' ')
                     {          // Special case for inkscape relative start
                        char *q = strchr (d + 2, ' ');
                        if (q && !isalpha (q[1]))
                           *q = 'l';
                        *d = 'M';
                     }
                     char *i = d,
                        *o = d;
                     while (*i)
                     {          // Compact
                        if ((isalpha (*i) && isspace (i[1])) || (isdigit (*i) && isspace (i[1]) && isalpha (i[2])))
                        {
                           *o++ = *i++;
                           i++;
                        } else
                           *o++ = *i++;
                     }
                     *o = 0;
                     fprintf (path, "%s", d);
                  }
               }
               fclose (path);
            }
         }
         if (d && *d)
            fprintf (f, "\t\"%s\"", d);
         else
            fprintf (f, "\tNULL");
         fprintf (f, ",\t// %s\n", card[c]);
         if (d)
            free (d);
      }
      fprintf (f, "};\n\n");
   }
   fprintf (f, "struct {int x,y,s,r,border;} pips[%d][%d]={\n", (int) (sizeof (pips) / sizeof (*pips)), (int) (sizeof (pips[0]) / sizeof (*pips[0])));
   for (c = 0; c < sizeof (card) / sizeof (*card); c++)
   {
      fprintf (f, "\t{");
      int n = 0;
      while (n < sizeof (pips[0]) / sizeof (*pips[0]) && pips[c][n].s)
      {
         fprintf (f, "%s{%d,%d,%d,%d", n ? "," : "", pips[c][n].x, pips[c][n].y, pips[c][n].s, pips[c][n].r);
         if (pips[c][n].border)
            fprintf (f, ",1");
         fprintf (f, "}");
         n++;
      }
      fprintf (f, "},\t// %s\n", card[c]);
   }
   fprintf (f, "};\n");
   fclose (f);
   return 0;
}
