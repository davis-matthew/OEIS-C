/*
 * A368708  a(n) = hypergeom([-1 - n, -n, 1 - n], [2, 3], -2).
 * https://oeis.org/A368708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (recurrence):
    a(0)=1, a(1)=1, a(2)=3
    a(n) = ( 3*(9*n^3+3*n^2-4*n+4)*(n+1)*a(n-1)
            + 3*(n-2)*(3*n-1)*(9*n^2-3*n-10)*a(n-2)
            + 27*(3*n+1)*(n-3)*(n-2)^2*a(n-3) )
          / ((n+1)*(n+3)*(n+2)*(3*n-2))
    (for n >= 3)
    See OEIS A368708 for details.
*/
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_upto = 0;   /* last index that has a value stored */

    /* allocate or grow cache if necessary */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cached_upto = 0;
        cache[0] = (seq_t)1;      /* initialise a(0) */
    } else if ((size_t)n + 1 > cache_cap) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

#if defined(__cplusplus)
    /* C++ mode – use the native seq_t type for all arithmetic */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)3;
        } else {
            seq_t N = (seq_t)i;
            /* left‑hand side: (N+1)*(N+3)*(N+2)*(3*N-2) */
            seq_t lhs = (N + 1) * (N + 3) * (N + 2) * (3 * N - 2);

            /* right‑hand side terms */
            seq_t rhs = 0;
            /* term1 */
            rhs += (seq_t)3 *
                   ( ((seq_t)9 * N * N * N) + ((seq_t)3 * N * N) - ((seq_t)4 * N) + (seq_t)4 )
                   * (N + 1) * cache[i - 1];
            /* term2 */
            rhs += (seq_t)3 *
                   ( (N - 2) * (3 * N - 1) *
                     ( ((seq_t)9 * N * N) - ((seq_t)3 * N) - (seq_t)10 ) )
                   * cache[i - 2];
            /* term3 */
            rhs += (seq_t)27 *
                   ( (3 * N + 1) * (N - 3) *
                     (N - 2) * (N - 2) )
                   * cache[i - 3];

            cache[i] = rhs / lhs;
        }
    }
#else
    /* C mode – use unsigned __int128 for intermediate larger than seq_t */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)3;
        } else {
            unsigned __int128 N = (unsigned __int128)i;

            /* left‑hand side: (N+1)*(N+3)*(N+2)*(3*N-2) */
            unsigned __int128 lhs =
                (N + 1) * (N + 3) * (N + 2) * (3 * N - 2);

            /* right‑hand side terms */
            unsigned __int128 rhs = 0;
            /* term1 */
            rhs += (unsigned __int128)3 *
                   ( ( (unsigned __int128)9 * N * N * N ) +
                     ( (unsigned __int128)3 * N * N ) -
                     ( (unsigned __int128)4 * N ) +
                     (unsigned __int128)4 )
                   * (N + 1) * (unsigned __int128)cache[i - 1];
            /* term2 */
            rhs += (unsigned __int128)3 *
                   ( (N - 2) * (3 * N - 1) *
                     ( (unsigned __int128)9 * N * N -
                       (unsigned __int128)3 * N -
                       (unsigned __int128)10 ) )
                   * (unsigned __int128)cache[i - 2];
            /* term3 */
            rhs += (unsigned __int128)27 *
                   ( (3 * N + 1) * (N - 3) *
                     (N - 2) * (N - 2) )
                   * (unsigned __int128)cache[i - 3];

            cache[i] = (seq_t)(rhs / lhs);
        }
    }
#endif

    cached_upto = n;
    return cache[n];
}
