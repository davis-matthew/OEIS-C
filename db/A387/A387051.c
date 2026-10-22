/*
 * A387051  Number of entries in the n-th row of Pascal's triangle not
 *          divisible by 32.
 * https://oeis.org/A387051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { k | 0 ≤ k ≤ n, 32 ∤ C(n,k) }
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses Kummer's theorem: v2(C(n,k)) = popcount(k)+popcount(n-k)-popcount(n) */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* number of entries in row n of Pascal's triangle not divisible by 32 */
    const int LIMIT = 5;          /* v2(C(n,k)) must be < 5 */
    const unsigned long long pc_n = __builtin_popcountll((unsigned long long)n);
    seq_t cnt = 0;                /* result, using seq_t type */

    for (idx_t k = 0; k <= n; ++k) {
        unsigned long long pc_k   = __builtin_popcountll((unsigned long long)k);
        unsigned long long pc_nk  = __builtin_popcountll((unsigned long long)(n - k));
        int v2 = (int)(pc_k + pc_nk - pc_n);
        if (v2 < LIMIT) {
            cnt = cnt + 1;        /* seq_t addition, using builtin operators */
        }
    }

    return cnt;
}
