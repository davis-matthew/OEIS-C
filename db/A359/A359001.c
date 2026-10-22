/*
 * A359001  Number of undirected n-cycles of the dodecahedral graph.
 * https://oeis.org/A359001
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of undirected simple n‑cycles of the dodecahedral
 *            graph
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required types are supplied by the build system. */
#include <stdbool.h>
#include <stddef.h>

/*--------------------------------------------------------------------
   Constants – coordinates of the 20 vertices of a regular dodecahedron
   with edge length approximately 1.2360679775 (the exact ratio is
   derived from the golden ratio φ = (1+√5)/2).  The coordinates are
   scaled so that all edge lengths are equal and the edges can be
   detected by checking for the smallest distance between any two
   vertices.  No floating‑point arithmetic is performed beyond these
   constant arrays, so the code remains compliant with the
   “no <math.h> on values” rule.
--------------------------------------------------------------------*/
static const double phi      = 1.6180339887498948482;   /* (1+√5)/2   */
static const double invphi   = 0.6180339887498948482;   /* 1/φ         */

#define V 20          /* number of vertices in the dodecahedral graph */
#define MAXN 20       /* longest possible simple cycle */

/* Geometry: coordinates of the 20 vertices.  They are listed in the
   order 0..19. */
static const double coords[V][3] = {
    {  1.0,   1.0,   1.0},   /* 0 */
    {  1.0,   1.0,  -1.0},   /* 1 */
    {  1.0,  -1.0,   1.0},   /* 2 */
    {  1.0,  -1.0,  -1.0},   /* 3 */
    { -1.0,   1.0,   1.0},   /* 4 */
    { -1.0,   1.0,  -1.0},   /* 5 */
    { -1.0,  -1.0,   1.0},   /* 6 */
    { -1.0,  -1.0,  -1.0},   /* 7 */
    {  0.0,  invphi,  phi},   /* 8 */
    {  0.0,  invphi, -phi},   /* 9 */
    {  0.0, -invphi,  phi},   /*10 */
    {  0.0, -invphi, -phi},   /*11 */
    { invphi,  phi,  0.0},   /*12 */
    { invphi, -phi,  0.0},   /*13 */
    {-invphi,  phi,  0.0},   /*14 */
    {-invphi, -phi,  0.0},   /*15 */
    { phi,   0.0,  invphi},   /*16 */
    { phi,   0.0, -invphi},   /*17 */
    {-phi,   0.0,  invphi},   /*18 */
    {-phi,   0.0, -invphi}    /*19 */
};

/*--------------------------------------------------------------------
   Graph construction
--------------------------------------------------------------------*/
static bool adjMat[V][V] = {0};          /* adjacency matrix */
static int  neigh[V][3] = {0};           /* list of neighbours, degree = 3 */
static int  neighCnt[V] = {0};

static void build_adjacency(void)
{
    double minDist = 1e100;
    /* Find the smallest non‑zero squared distance between any two vertices. */
    for (int i = 0; i < V; ++i) {
        for (int j = i + 1; j < V; ++j) {
            double dx = coords[i][0] - coords[j][0];
            double dy = coords[i][1] - coords[j][1];
            double dz = coords[i][2] - coords[j][2];
            double d2 = dx*dx + dy*dy + dz*dz;
            if (d2 > 0.0 && d2 < minDist)
                minDist = d2;
        }
    }

    const double eps = 1e-8;   /* tolerance for floating point comparisons */
    /* Construct the adjacency structure. */
    for (int i = 0; i < V; ++i) {
        for (int j = i + 1; j < V; ++j) {
            double dx = coords[i][0] - coords[j][0];
            double dy = coords[i][1] - coords[j][1];
            double dz = coords[i][2] - coords[j][2];
            double d2 = dx*dx + dy*dy + dz*dz;
            if (d2 <= minDist + eps) {
                adjMat[i][j] = adjMat[j][i] = true;
                neigh[i][neighCnt[i]++] = j;
                neigh[j][neighCnt[j]++] = i;
            }
        }
    }
}

/*--------------------------------------------------------------------
   Cycle enumeration (DFS)
--------------------------------------------------------------------*/
static unsigned char path[MAXN];      /* current path during DFS */
static seq_t         cycleCount[MAXN+1] = {0};
static bool          initialized = false;

/* Recursive DFS that counts all simple cycles starting at vertex `start`. */
static void dfs_enum(int start, int depth, unsigned int mask, int minSeen)
{
    /* Check for a cycle when the path length is at least 5. */
    if (depth >= 5) {
        int cur = path[depth-1];
        if (adjMat[cur][start] && path[1] < cur && minSeen == start) {
            cycleCount[depth] = cycleCount[depth] + 1;
        }
    }
    if (depth == MAXN) return;   /* cannot extend further */
    int cur = path[depth-1];
    for (int k = 0; k < 3; ++k) {
        int v = neigh[cur][k];
        if ((mask & (1U << v)) == 0) {   /* vertex not yet visited */
            path[depth] = (unsigned char)v;
            dfs_enum(start,
                     depth + 1,
                     mask | (1U << v),
                     minSeen < v ? minSeen : v);
        }
    }
}

/*--------------------------------------------------------------------
   Public function requested by the OEIS entry.
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (!initialized) {
        build_adjacency();
        for (int s = 0; s < V; ++s) {
            path[0] = (unsigned char)s;
            dfs_enum(s, 1, 1U << s, s);
        }
        initialized = true;
    }
    if (n >= 5 && (idx_t)MAXN >= n)
        return cycleCount[n];
    return 0;
}
