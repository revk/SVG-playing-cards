// Random puzzle maze box

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
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

int
main (int argc, const char *argv[])
{
  int wallthickness = 3;
  int insidewall = 2;
  int insidediameter = 30;
  int insideheight = 60;
  int extraheight = 0;
  int baseheight = 8;
  int basediameter = 0;
  int mazespacing = 5;
  int paths = 0;
  int lid = 0;
  int base = 0;
  int ring = -1;
  int insiderussian = 0;	// How many mazes will be inside this one
  int outsiderussian = 0;	// How many mazes will be outside this one
  int basesides = 0;
  int skew = 0;
  int inside = 0;
  const char *margin = "0.2";
  const char *scad = "/projects/tools/script/puzzlebox.scad";

  const char *pathinfo = getenv ("PATH_INFO");
  if (pathinfo)
    {
      const char *p = strrchr (pathinfo, '/');
      if (p && *p == '/' && isalpha (p[1]))
	{
	  p++;
	  while (*p && *p != '/')
	    {
	      if (*p == 'l')
		lid = 1;
	      else if (*p == 'b')
		base = 1;
	      else if (*p == 'r')
		ring = 1;
	      else if (*p == 'n')
		ring = 0;
	      else if (*p == 'i')
		insiderussian++;
	      else if (*p == 'o')
		outsiderussian++;
	      else if (*p == 'k')
		skew = 1 - skew;
	      else if (*p == 'i')
		inside = 1 - inside;
	      p++;
	    }
	}
      p = pathinfo;
      if (p && *p == '/')
	{
	  if (isdigit (p[1]))
	    insideheight = atoi (p + 1);
	  p = strchr (p + 1, '/');
	}
      if (p && *p == '/')
	{
	  if (isdigit (p[1]))
	    insidediameter = atoi (p + 1);
	  p = strchr (p + 1, '/');
	}
      if (p && *p == '/')
	{
	  if (isdigit (p[1]))
	    extraheight = atoi (p + 1);
	  p = strchr (p + 1, '/');
	}
      if (p && *p == '/')
	{
	  if (isdigit (p[1]))
	    paths = atoi (p + 1);
	  p = strchr (p + 1, '/');
	}
      if (p && *p == '/')
	{
	  if (isdigit (p[1]))
	    baseheight = atoi (p + 1);
	  p = strchr (p + 1, '/');
	}
      if (p && *p == '/')
	{
	  if (isdigit (p[1]))
	    wallthickness = atoi (p + 1);
	  p = strchr (p + 1, '/');
	}
      if (p && *p == '/')
	{
	  if (isdigit (p[1]))
	    mazespacing = atoi (p + 1);
	  p = strchr (p + 1, '/');
	}
      if (p && *p == '/')
	{
	  if (isdigit (p[1]))
	    basediameter = atoi (p + 1);
	  p = strchr (p + 1, '/');
	}
    }
  {
    int c;
    poptContext optCon;		// context for parsing command-line options
    const struct poptOption optionsTable[] = {
      {"base-height", 0, POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT, &baseheight, 0, "Base height", "mm"},
      {"base-diameter", 0, POPT_ARG_INT, &basediameter, 0, "Base diameter", "mm"},
      {"base-sides", 0, POPT_ARG_INT, &basesides, 0, "Base sides", "N"},
      {"inside-diameter", 0, POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT, &insidediameter, 0, "Inside diameter", "mm"},
      {"inside-height", 0, POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT, &insideheight, 0, "Inside height", "mm"},
      {"extra-height", 0, POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT, &extraheight, 0, "Extra height", "mm"},
      {"inside-wall", 0, POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT, &insidewall, 0, "Min maze wall thickness", "mm"},
      {"maze-spacing", 0, POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT, &mazespacing, 0, "Maze spacing", "mm"},
      {"wall-thickness", 0, POPT_ARG_INT | POPT_ARGFLAG_SHOW_DEFAULT, &wallthickness, 0, "Lid wall thickness", "mm"},
      {"paths", 0, POPT_ARG_INT, &paths, 0, "Paths", "N"},
      {"margin", 0, POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &margin, 0, "Margin", "mm"},
      {"lid", 0, POPT_ARG_NONE, &lid, 0, "Lid only"},
      {"ring", 0, POPT_ARG_NONE, &ring, 0, "Ring around the middle"},
      {"base", 0, POPT_ARG_NONE, &base, 0, "Base only"},
      {"skew", 0, POPT_ARG_NONE, &skew, 0, "Skew/twist"},
      {"inside", 0, POPT_ARG_NONE, &inside, 0, "Maze on inside of lid"},
      {"inside", 0, POPT_ARG_INT, &insiderussian, 0, "Has inside Russian dolls", "N"},
      {"outside", 0, POPT_ARG_INT, &outsiderussian, 0, "Has outside Russian dolls", "N"},
      {"scad", 0, POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &scad, 0, "SCAD file", "path"},
      POPT_AUTOHELP {}
    };

    optCon = poptGetContext (NULL, argc, argv, optionsTable, 0);
    //poptSetOtherOptionHelp (optCon, "");

    if ((c = poptGetNextOpt (optCon)) < -1)
      errx (1, "%s: %s\n", poptBadOption (optCon, POPT_BADOPTION_NOALIAS), poptStrerror (c));

    if (poptPeekArg (optCon))
      {
	poptPrintUsage (optCon, stderr, 0);
	return -1;
      }
    poptFreeContext (optCon);
  }

  // Sanity checking
  if (insideheight < 10)
    insideheight = 10;
  if (insideheight > 200)
    insideheight = 200;
  if (insidediameter < 10)
    insidediameter = 10;
  if (insidediameter > 100)
    insidediameter = 100;
  if (extraheight < 0)
    extraheight = 0;
  if (extraheight > 100)
    extraheight = 100;
  if (mazespacing < 2)
    mazespacing = 2;
  if (mazespacing > 20)
    mazespacing = 20;
  if (wallthickness < 0)
    wallthickness = 0;
  if (wallthickness > 10)
    wallthickness = 10;
  if (insidewall < 0)
    insidewall = 0;
  if (insidewall > 10)
    insidewall = 10;
  if (baseheight < wallthickness)
    baseheight = wallthickness;
  if (baseheight > insideheight + wallthickness - 8)
    baseheight = insideheight + wallthickness - 8;

  if (insiderussian)
    insideheight += extraheight + wallthickness * insiderussian - baseheight;

  int h = (insideheight + wallthickness - baseheight - 3) / mazespacing;
  int w = (insidediameter + insidewall * 2 + mazespacing) * 31415 / 10000 / mazespacing;

  if (ring < 0)
    ring = (h > 20);		// Was unset, so do ring if tall enough

  if (!paths)
    {				// Default paths
      paths = (w + 15) / 15;
      if (paths < 1)
	paths = 1;
    }

  if (paths < 1)
    paths = 1;
  if (paths > 10 || paths > w / 2)
    paths = w / 2;

  insidediameter += insiderussian * (insidewall * 2 + mazespacing);	// We have to be wider as we have others inside

  if (insiderussian)
    extraheight = baseheight;	// The base of the inside is inside this one so need extra height

  if (basediameter < insidediameter + insidewall * 2 + mazespacing + wallthickness * 2)
    basediameter = insidediameter + insidewall * 2 + mazespacing + wallthickness * 2;

  if (outsiderussian)
    {				// Base has to fit inside another layer
      base = 1;
      lid = 0;
      basediameter = insidediameter + 2 * (mazespacing + insidewall * 2);
    }

  if (!basesides)
    basesides = (insidediameter + 4) / 4;
  if (basesides > 12)
    basesides = 12;
  if (basesides < 3)
    basesides = 3;
  basesides = (basesides + paths - 1) / paths * paths;
  if (basesides < 3)
    basesides += paths;

  w = (insidediameter + insidewall * 2 + mazespacing) * 31415 / 10000 / mazespacing / paths * paths;

  int dx = w / paths, ox = 0;

#define	R 1
#define U 2
#define L 4
#define D 8
  //int topl = 0, topx = 0, basel = 0, basex = 0;
  unsigned char maze[h][w];
  memset (&maze, 0, w * h);
  typedef struct q_s q_t;
  struct q_s
  {
    int x,			// Position
      y;
  };
  int qn = 0, x, y;
  q_t q[w * h];
  memset (&q, 0, sizeof (*q) * w * h);
  int r = open ("/dev/urandom", O_RDONLY);
  if (r < 0)
    err (1, "Cannot open random");
  y = h / 2;
  if (ring)
    {				// Ring around middle
      for (x = 0; x < dx; x++)
	{
	  q[qn].y = y;
	  q[qn].x = x;
	  qn++;
	}
      for (x = 0; x < w; x++)
	maze[y][x] = (R | L);
    }
  else
    {				// Start in middle
      q[qn].y = y;
      qn++;
    }
  while (qn)
    {
      unsigned int n = 0;
      if (read (r, &n, sizeof (n)) != sizeof (n))
	err (1, "Bad random read");
      if (n & 0x3000)
	n = qn - 1;		// carry on from newest node a proportion of the time
      else
	n %= qn;		// Pick a node
      x = q[n].x;
      y = q[n].y;
      unsigned char o = 0,	// where we can go
	c = 0;			// how many options
      if (!(maze[y][x] & R) && !maze[y][(x + 1) % w])
	{			// Can go right
	  o |= R;
	  c++;
	}
      if (!(maze[y][x] & L) && !maze[y][(x + w - 1) % w])
	{			// Can go left
	  o |= L;
	  c++;
	}
      if (y < h - 1 && !(maze[y][x] & U) && !maze[y + 1][x])
	{			// Can go up
	  o |= U;
	  c++;
	}
      if (y > 0 && !(maze[y][x] & D) && !maze[y - 1][x])
	{			// Can do down
	  o |= D;
	  c++;
	}
      if (c <= 1)
	{			// Remove node
	  if (n + 1 < qn)
	    memmove (q + n, q + n + 1, (qn - n) * sizeof (*q));
	  qn--;
	  if (!c)
	    continue;		// Cannot move at all
	}
      // Random choice
      unsigned int d = 0;
      if (read (r, &d, sizeof (d)) != sizeof (d))
	err (1, "Bad random read");
      if (!(d & 0x3000))
	{			// bias to going straight on (if we can)
	  if ((maze[y][x] & (L << 4)) && (o & R))
	    o = R, d = 0;
	  else if ((maze[y][x] & (R << 4)) && (o & L))
	    o = L, d = 0;
	  else if ((maze[y][x] & (D << 4)) && (o & U))
	    o = U, d = 0;
	  else if ((maze[y][x] & (U << 4)) && (o & D))
	    o = D, d = 0;
	}
      // Move 
      if (d && c)
	d %= c;			// Random choice
      //unsigned char was = maze[y][x];
      if ((o & R) && !d--)
	{			// Go Right
	  for (ox = 0; ox < w; ox += dx)
	    maze[y][(x + ox) % w] |= R;
	  x = (x + 1) % w;
	  o = L;
	}
      else if ((o & L) && !d--)
	{			// Go Left
	  for (ox = 0; ox < w; ox += dx)
	    maze[y][(x + ox) % w] |= L;
	  x = (x + w - 1) % w;
	  o = R;
	}
      else if ((o & U) && !d--)
	{			// Go Up
	  for (ox = 0; ox < w; ox += dx)
	    maze[y][(x + ox) % w] |= U;
	  y++;
	  o = D;
	}
      else if ((o & D) && !d--)
	{			// Go Down
	  for (ox = 0; ox < w; ox += dx)
	    maze[y][(x + ox) % w] |= D;
	  y--;
	  o = U;
	}
      else
	errx (1, "WTF");
      if (!maze[y][x])
	{			// New node
	  for (ox = 0; ox < w; ox += dx)
	    maze[y][(x + ox) % w] = (o | (o << 4));
	  q[qn].x = x;
	  q[qn].y = y;
	  qn++;
	}
    }
  close (r);
  int bests = 0, beste = 0;
  {				// Maze scoring to find best start/exit point
    int bestl = 0, s;
    unsigned int score[h][w];
    for (s = 0; s < dx; s++)
      {				// Try start points
	qn = 0;
	for (y = 0; y < h; y++)
	  for (x = 0; x < w; x++)
	    score[y][x] = INT_MAX;
	score[0][s] = 0;
	q[qn].x = s;
	q[qn].y = 0;
	qn++;
	while (qn)
	  {
	    x = q[qn - 1].x;
	    y = q[qn - 1].y;
	    int next = score[y][x];
	    unsigned char o = (maze[y][x] & 15);
	    // new score
	    next++;		// moving forward
	    if (o == L + R + U || o == L + R + D || o == R + U + D || o == L + U + D)
	      next += 10;
	    else if (o == L + R + U + D)
	      next += 20;
	    //fprintf (stderr, "%d: %d,%d %d %X\n", qn, x, y, next, o);
	    if ((o & L) && score[y][(x + w - 1) % w] > next)
	      x = ((x + w - 1) % w);
	    else if ((o & R) && score[y][(x + 1) % w] > next)
	      x = ((x + 1) % w);
	    else if ((o & U) && y < h - 1 && score[y + 1][x] > next)
	      y++;
	    else if ((o & D) && y > 0 && score[y - 1][x] > next + 5)
	      y--, next += 5;
	    else
	      {			// back
		qn--;
		continue;
	      }
	    // Forward
	    q[qn].x = x;
	    q[qn].y = y;
	    qn++;
	    score[y][x] = next;
	  }
	int bestx = 0, best = 0;
	for (x = 0; x < w; x++)
	  {
	    if (score[h - 1][x] > best && score[h - 1][x] < INT_MAX)
	      best = score[h - 1][bestx = x];
	  }
	//fprintf (stderr, "st=%d ex=%d sc=%d\n", s, bestx, best);
	if (best > bestl)
	  bests = s, beste = bestx, bestl = best;
      }
  }
  printf ("Content-Type: application/openscad\n");
  printf ("Content-Disposition: attachment; filename=labarynth.scad\n");
  printf ("\n");
  printf ("// Puzzle box by Adrian Kennard www.me.uk\n");
  if (pathinfo)
    printf ("// %s\n", pathinfo);
  printf ("p=%d;	// Paths\n", paths);
  printf ("m=%s;	// Margin/clearance\n", margin);
  printf ("s=%d;	// Spacing (unit)\n", mazespacing);
  printf ("h=%d;	// Height in units\n", h);
  printf ("w=%d;	// Width in units\n", w);
  printf ("bs=%d;	// Base sides\n", basesides);
  printf ("bh=%d;	// Base height\n", baseheight);
  printf ("bd=%d;	// Base diameter\n", basediameter);
  printf ("ih=%d;	// Inside height\n", insideheight);
  printf ("eh=%d;	// Extra height\n", extraheight);
  printf ("id=%d;	// Inside diameter\n", insidediameter);
  printf ("wt=%d;	// Wall thickness\n", wallthickness);
  printf ("iw=%d;	// Inside wall thickness\n", insidewall);
  printf ("st=%d;	// Start point\n", bests);
  printf ("ex=%d;	// Exit point\n", beste);
  printf ("lid=%d;	// Print lid\n", !base);
  printf ("base=%d;	// Print base\n", !lid);
  printf ("is=%d;	// Inside Russian\n", insiderussian);
  printf ("os=%d;	// Outside Russian\n", outsiderussian);
  printf ("k=%d;	// Skew\n", skew);
  printf ("i=%d;	// Inside (maze inside lid)\n", inside);
  printf ("maze=[\n");
  for (y = 0; y < h; y++)
    {
      int x;
      printf ("[");
      for (x = 0; x < w; x++)
	printf ("%d%s", maze[y][x] & 3, (x + 1 < w) ? "," : "");
      printf ("]%s", (y + 1 < h) ? ",\n" : "");
    }
  printf ("];\n");
  printf ("\n");
  FILE *i = fopen (scad, "r");
  if (!i)
    err (1, "Cannot open %s", scad);
  char buf[1000];
  while (fgets (buf, sizeof (buf), i) && *buf >= ' ');	// Skip to blank line
  while (fgets (buf, sizeof (buf), i))
    printf ("%s", buf);
  fclose (i);
}
