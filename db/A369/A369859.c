/*
 * A369859  Minimum chord of a chordal ring graph with 2*n vertices and
 *          minimal diameter.
 * https://oeis.org/A369859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest odd chord length c that yields a chordal
 *            ring graph CR(2n,c) with the smallest possible diameter
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

/* External types defined by the build */
extern seq_t a(idx_t n);

/* Internal memoization cache */
static seq_t *cache_vals = NULL;
static idx_t cache_cap = 0;

/* Helper: allocate or grow cache to at least size n+1 and zero-initialize new entries */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
    /* initialize new entries to 0 */
    for (idx_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    cache_vals = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, but return 0 safely */

    /* Use cached value if available */
    ensure_cache(n);
    if (cache_vals[n] != 0) return cache_vals[n];

    /* Parameters */
    idx_t m = n * 2;                 /* number of vertices = 2n */
    size_t ms = (size_t)m;           /* safe for small n (<= 10^5) */

    /* Allocate shared arrays for BFS */
    int *dist = (int *)malloc(ms * sizeof(int));
    idx_t *queue = (idx_t *)malloc(ms * sizeof(idx_t));

    /* Variables to track best result */
    int bestDiameter = INT_MAX;
    idx_t bestChord = 0;

    /* Iterate over odd chord lengths c */
    for (idx_t c = 1; c < m; c += 2) {
        /* Initialize BFS */
        memset(dist, -1, ms * sizeof(int));
        size_t head = 0, tail = 0;
        dist[0] = 0;
        queue[tail++] = 0;
        int curMaxDist = 0;
        int prune = 0;   /* flag to abort if distance exceeds best known */

        while (head < tail && !prune) {
            idx_t u = queue[head++];
            int du = dist[u];

            /* Neighbor 1: u+1 mod m */
            idx_t v = (u + 1 < m) ? (u + 1) : 0;
            if (dist[v] == -1) {
                dist[v] = du + 1;
                if (dist[v] > bestDiameter) { prune = 1; break; }
                if (dist[v] > curMaxDist) curMaxDist = dist[v];
                queue[tail++] = v;
            }

            /* Neighbor 2: u-1 mod m */
            if (prune) break;
            v = (u > 0) ? (u - 1) : (m - 1);
            if (dist[v] == -1) {
                dist[v] = du + 1;
                if (dist[v] > bestDiameter) { prune = 1; break; }
                if (dist[v] > curMaxDist) curMaxDist = dist[v];
                queue[tail++] = v;
            }

            /* Neighbor 3: chord */
            if (!prune) {
                if ((u & 1) == 1) {            /* u is odd: chord to u+c */
                    v = u + c;
                    if (v >= m) v -= m;
                } else {                        /* u is even: chord to u-c */
                    if (u >= c) v = u - c;
                    else v = u + m - c;
                }
                if (dist[v] == -1) {
                    dist[v] = du + 1;
                    if (dist[v] > bestDiameter) { prune = 1; break; }
                    if (dist[v] > curMaxDist) curMaxDist = dist[v];
                    queue[tail++] = v;
                }
            }
        }

        if (prune) continue;  /* this chord gives a larger diameter */

        /* Update best result */
        if (curMaxDist < bestDiameter) {
            bestDiameter = curMaxDist;
            bestChord = c;
            if (bestDiameter == 1) break;  /* cannot improve further */
        } else if (curMaxDist == bestDiameter && c < bestChord) {
            bestChord = c;
        }
    }

    /* Clean up */
    free(dist);
    free(queue);

    /* Store and return result */
    cache_vals[n] = bestChord;
    return bestChord;
}
