/*
 * A309082  a(n) = n - floor(n/2^3) + floor(n/3^3) - floor(n/4^3) + ...
 * https://oeis.org/A309082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n^(1/3))} (-1)^(k+1) * floor(n/k^3)
 * Range:     u8 n <= 282, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_cube_root.h"

/* Returns the value a(n) of OEIS A309082 for n >= 1.  The sequence is
 * defined by a(n) = n - ⌊n/2^3⌋ + ⌊n/3^3⌋ - ⌊n/4^3⌋ + ... , which
 * is equivalently a(n) = Σ_{k=1}^{⌊n^{1/3}⌋} (-1)^{k+1} ⌊n/k^3⌋.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* defined only for n >= 1 */
    seq_t sn = (seq_t)n;                 /* cast index to seq_t once */

    idx_t kmax = int_cube_root(n);       /* largest k with k^3 <= n */
    seq_t sum_pos = 0;                   /* Σ for odd k ( + ) */
    seq_t sum_neg = 0;                   /* Σ for even k ( - ) */

    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t kk = (seq_t)k * (seq_t)k * (seq_t)k;  /* k^3 */
        seq_t term = sn / kk;               /* ⌊n/k^3⌋ */
        if (k & 1U)                        /* odd k => add */
            sum_pos += term;
        else                               /* even k => subtract */
            sum_neg += term;
    }

    return sum_pos - sum_neg;             /* sum_pos >= sum_neg always */
}
