/*
 * A379084  a(n) = Sum_{k=0..floor(n/2)} binomial(2*n+k-1,k) *
 *          binomial(2*n+k,n-2*k).
 * https://oeis.org/A379084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(2*n + k - 1, k) * binomial(2*n
 *            + k, n - 2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* `seq_t` and `idx_t` are defined by the build system. */
#include "../utils/binom.h"

/* Computes the OEIS sequence A379084. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                      /* a(0) = 1 */

    seq_t sum = 0;                     /* accumulator for the sum */

    for (idx_t k = 0; k <= n / 2; ++k) {
        /* binom arguments are non‑negative because k <= n/2. */
        idx_t first_n   = 2 * n + k;            /* 2*n + k   */
        idx_t first_k   = k;                    /* k         */
        idx_t second_n  = first_n;              /* 2*n + k   */
        idx_t second_k  = n - 2 * k;            /* n - 2*k   */

        seq_t t1 = binom(first_n - 1, first_k);   /* binomial(2*n + k - 1, k) */
        seq_t t2 = binom(second_n, second_k);     /* binomial(2*n + k, n - 2*k) */

        sum += t1 * t2;
    }

    return sum;
}
