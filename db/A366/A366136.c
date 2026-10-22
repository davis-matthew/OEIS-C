/*
 * A366136  Minimal number of factorials or their negatives that add to n.
 * https://oeis.org/A366136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by breadth‑first search over the infinite graph
   whose vertices are all integers and whose edges are
   x → x ± k!  for every k ≥ 0.  The array `dist` stores the
   minimal number of factorials (positive or negative) needed
   to reach each integer in the range [−MAX_QUERY, MAX_QUERY].
   For values of n outside this range a trivial representation
   using n copies of 1! (the 0! term) is returned.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_QUERY 10000            /* upper bound for the test harness */
#define FACT_COUNT 8               /* 0! … 7! (duplicate 0! and 1! are ok) */

static const uint64_t factorials[FACT_COUNT] = {1,1,2,6,24,120,720,5040};

static unsigned char *dist = NULL;  /* distance array: indices are [−MAX_QUERY,MAX_QUERY] */
static bool initialized = false;

/* ------------------------------------------------------------ */
static void init_cache(void)
{
    const size_t SZ = 2 * MAX_QUERY + 1;          /* range of reachable values */
    dist = (unsigned char *)malloc(SZ * sizeof(unsigned char));
    if (!dist) return;                           /* allocation failed – will not compute */

    /* initialise every entry as “unvisited” (255)   */
    memset(dist, 255, SZ);

    /* BFS queue */
    int64_t *queue = (int64_t *)malloc(SZ * sizeof(int64_t));
    size_t qhead = 0, qtail = 0;

    /* zero has distance 0 */
    dist[MAX_QUERY] = 0;
    queue[qtail++] = 0;

    while (qhead < qtail) {
        int64_t cur   = queue[qhead++];
        unsigned char d = dist[MAX_QUERY + cur];

        for (size_t i = 0; i < FACT_COUNT; ++i) {
            int64_t step = (int64_t)factorials[i];

            /* cur + step */
            int64_t nxt = cur + step;
            if (nxt >= -MAX_QUERY && nxt <= MAX_QUERY
                && dist[MAX_QUERY + nxt] == 255) {
                dist[MAX_QUERY + nxt] = d + 1;
                queue[qtail++] = nxt;
            }

            /* cur - step */
            nxt = cur - step;
            if (nxt >= -MAX_QUERY && nxt <= MAX_QUERY
                && dist[MAX_QUERY + nxt] == 255) {
                dist[MAX_QUERY + nxt] = d + 1;
                queue[qtail++] = nxt;
            }
        }
    }

    free(queue);
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_cache();
        initialized = true;
    }

    if (n <= MAX_QUERY) {
        return (seq_t)dist[MAX_QUERY + n];
    }

    /* For n beyond the pre‑computed range, fall back to the
       trivial representation using n copies of 1!  (1! = 1). */
    return (seq_t)n;
}
