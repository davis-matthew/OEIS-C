/*
 * A290801  a(n) is the least number of steps to get to n from 0 using only
 *          the operations of incrementation, decrementation, and squaring.
 * https://oeis.org/A290801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) is the shortest number of operations (+1, -1, x²) needed to
 *            reach n from 0. Computation is performed by a breadth‑first
 *            search limited to numbers ≤ (⌊√n⌋+1)², which is sufficient to
 *            find an optimal path
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/* Integer square root for 64‑bit unsigned values.
   Returns ⌊√n⌋.  Uses binary search with 128‑bit arithmetic
   to avoid overflow. */
static inline idx_t sqrt_u64(idx_t n)
{
    idx_t low = 0, high = 0xFFFFFFFFULL;        /* √(2⁶⁴−1) < 2³² */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Compute a(n) by BFS up to a reasonable bound. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Upper bound that guarantees a shortest path is found
       (deepest visited node is (⌊√n⌋+1)²). */
    idx_t root = sqrt_u64(n);
    idx_t limit = (root + 1) * (root + 1);
    if (limit < n)
        limit = n;                 /* safety for tiny n */

    size_t size = (size_t)limit + 1;
    size_t *dist  = (size_t *)malloc(size * sizeof(size_t));
    size_t *queue = (size_t *)malloc(size * sizeof(size_t));
    if (!dist || !queue) {
        /* Allocation failed – return 0 as a safe default. */
        free(dist);
        free(queue);
        return (seq_t)0;
    }

    for (size_t i = 0; i < size; ++i)
        dist[i] = SIZE_MAX;        /* “∞” */

    size_t head = 0, tail = 0;
    queue[tail++] = 0;
    dist[0] = 0;

    while (head < tail)
    {
        size_t x = queue[head++];
        size_t d = dist[x] + 1;

        /* x + 1 */
        if (x + 1 < size && dist[x + 1] == SIZE_MAX) {
            dist[x + 1] = d;
            queue[tail++] = x + 1;
        }

        /* x - 1 */
        if (x > 0 && dist[x - 1] == SIZE_MAX) {
            dist[x - 1] = d;
            queue[tail++] = x - 1;
        }

        /* x² */
        unsigned __int128 sq128 = (unsigned __int128)x * x;
        if (sq128 < size) {
            size_t sq = (size_t)sq128;
            if (dist[sq] == SIZE_MAX) {
                dist[sq] = d;
                queue[tail++] = sq;
            }
        }
    }

    size_t result = dist[n];
    free(dist);
    free(queue);
    return (seq_t)result;
}
