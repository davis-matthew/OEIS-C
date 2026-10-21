/*
 * A294703  Number of permutations of [n] avoiding {1324, 2341, 4123}.
 * https://oeis.org/A294703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = catalan(n) - 3*n^3/8 + 3*n^2/2 - 17*n/15 + 3 + n^5/120 +
 *            2^(n-1)*(n-4) (n>0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n == 0) return 1ULL;
    if (n == 1) return 1ULL;
    if (n == 2) return 2ULL;
    if (n == 3) return 6ULL;

    /* n is now at least 4, so (n-4) is non‑negative */
    seq_t N   = (seq_t)n;             /* promote index to value type */
    seq_t n2  = N * N;                /* n^2 */
    seq_t n3  = n2 * N;               /* n^3 */
    seq_t n5  = n3 * n2;              /* n^5 = n^3 * n^2 */

    /* Catalan number */
    seq_t c = catalan(n);

    /* 2^(n-1) * (n-4) */
    seq_t pow2_val = pow2(n - 1);
    seq_t term_pow2 = pow2_val * (N - 4);    /* pow2(n-1)*(n-4) */

    /* Positive and negative polynomial parts (scaled by 120 later) */
    seq_t pos  = 120 * c + n5 + 120 * term_pow2 + 180 * n2 + 360;
    seq_t neg  = 45 * n3 + 136 * N;

    /* All positive terms exceed all negative terms for n >= 4 */
    seq_t numerator = pos - neg;

    return numerator / 120;
}
