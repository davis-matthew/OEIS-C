/*
 * A370672  Number of ways of arranging 2n+1 nonattacking queens on a 2n+1 X
 *          2n+1 toroidal board using knight moves.
 * https://oeis.org/A370672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 0 if (2*n+1) is a multiple of 3. * Otherwise let m =
 *            2*n+1. * Factor m = ∏ p_i^{e_i}. * The number of arrangements is
 *            * a(n) = ∏ p_i^{2*e_i-1} * (p_i - 3). * This product is taken
 *            over all prime power factors of m
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t m = 2*n + 1;                 /* board size */
    if (m % 3 == 0) return 0;          /* no solutions when divisible by 3 */
    seq_t result = 1;
    idx_t tmp = m;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            idx_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            /* compute p^(2*e-1) */
            seq_t pow_pe = 1;
            for (idx_t i = 0; i < 2*e-1; ++i)
                pow_pe *= (seq_t)p;
            seq_t term = pow_pe * (seq_t)(p-3);
            result *= term;
        }
    }
    if (tmp > 1)   /* remaining prime factor */
    {
        idx_t p = tmp;
        seq_t term = (seq_t)p * (seq_t)(p-3);   /* e = 1 -> p^(1)*(p-3) */
        result *= term;
    }
    return result;
}
