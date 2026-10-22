/*
 * A327606  Expansion of e.g.f. exp(x)*(1-x)*x/(1-2*x)^2.
 * https://oeis.org/A327606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{q=1..n} (n)_q * c_q, where c_q = 1 for q = 1, c_q =
 *            (q+1)·2^{q-2} for q >= 2. (n)_q denotes the falling factorial
 *            n·(n-1)…(n-q+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t prod = 1;          /* current falling factorial (n)_q-1 */
    seq_t a_val = 0;
    seq_t two_pow = 1;       /* 2^{q-2}, valid for q >= 2 */

    for (idx_t q = 1; q <= n; ++q) {
        /* update falling factorial: prod *= (n - q + 1) */
        prod *= (seq_t)(n - q + 1);

        seq_t term;
        if (q == 1) {
            term = prod;                     /* c_1 = 1 */
        } else {
            /* two_pow currently equals 2^{q-2} */
            term = prod * ((seq_t)(q + 1) * two_pow);
        }
        a_val += term;

        if (q >= 2) {
            /* prepare two_pow for next iteration */
            two_pow <<= 1;   /* multiply by 2 */
        }
    }
    return a_val;
}
