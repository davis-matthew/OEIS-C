/*
 * A309997  Number of paths from 2 to n of length A307092(n) - 1 via maps of
 *          the form x -> x + x^j, where j is a nonnegative integer.
 * https://oeis.org/A309997
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute the minimal number of maps from 2 to n of the form
   x -> x + x^j (j >= 0) and count the number of paths of that minimal
   length.  The function works for any n >= 2 by dynamic programming
   over the increasing sequence of nodes. */

/* Note: seq_t and idx_t are provided by the build system.  No external
   helper functions are needed. */

#include <stdlib.h>
#include <stddef.h>
#include <limits.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* undefined for indices < 2 */

    size_t N = (size_t)n;           /* array size */
    idx_t  *dist = (idx_t *)malloc((N + 1) * sizeof(idx_t));
    seq_t  *cnt  = (seq_t *)malloc((N + 1) * sizeof(seq_t));

    if (!dist || !cnt) return 0;    /* allocation failure -> 0 */

    /* initialise */
    for (size_t i = 0; i <= N; ++i) {
        dist[i] = (idx_t)(N + 1);   /* sentinel for "infinity" */
        cnt[i]  = 0;
    }
    dist[2] = 0;
    cnt[2]  = 1;

    for (size_t x = 2; x <= N; ++x) {
        if (dist[x] > N) continue;  /* unreachable; should not happen */

        idx_t d = dist[x];

        /* j = 0 : x + 1 */
        if (x + 1 <= N) {
            size_t y = x + 1;
            idx_t nd = d + 1;
            if (nd < dist[y]) {
                dist[y] = nd;
                cnt[y]  = cnt[x];
            } else if (nd == dist[y]) {
                cnt[y] += cnt[x];
            }
        }

        /* j >= 1 : x + x^j */
        unsigned long long power = x;   /* x^1 */
        for (;;) {
            /* if y would overflow unsigned long long, the loop will be
               terminated as y > N anyway */
            unsigned long long y = x + power;
            if (y > N) break;
            size_t yi = (size_t)y;
            idx_t nd = d + 1;
            if (nd < dist[yi]) {
                dist[yi] = nd;
                cnt[yi]  = cnt[x];
            } else if (nd == dist[yi]) {
                cnt[yi] += cnt[x];
            }

            /* Prepare next power = power * x, guarding against overflow */
            if (power > (ULLONG_MAX / x))
                power = ULLONG_MAX;    /* will terminate next iteration */
            else
                power *= x;
        }
    }

    seq_t result = cnt[N];
    free(dist);
    free(cnt);
    return result;
}
