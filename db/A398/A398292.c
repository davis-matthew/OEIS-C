/*
 * A398292  Diagonal of rational function 1/(1 - (x + y + z + w + x^2*y*z*w)).
 * https://oeis.org/A398292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (4*k)!/k!^4 * binomial(k, n-k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Start index of summation (ceil(n/2)). */
    idx_t k_start = (n + 1) / 2;

    /* Compute C = (4*k)!/(k!^4) up to k = k_start. */
    seq_t C = 1;
    for (idx_t k = 0; k < k_start; ++k) {
        unsigned int kk = (unsigned int)(k + 1);          /* k+1 <= 10001 */
        C = mul_small(C, 4 * kk);   C /= kk;
        C = mul_small(C, 4 * kk - 1); C /= kk;
        C = mul_small(C, 4 * kk - 2); C /= kk;
        C = mul_small(C, 4 * kk - 3); C /= kk;
    }

    /* Initial binomial term for m = n - k_start. */
    idx_t m_start = n - k_start;
    seq_t B = 1;
    for (idx_t i = 1; i <= m_start; ++i) {
        unsigned int fac = (unsigned int)(k_start - m_start + i);
        B = mul_small(B, fac);    B /= (unsigned int)i;
    }

    /* Accumulate the sum. */
    seq_t sum = 0;
    for (idx_t k = k_start; k <= n; ++k) {
        sum += C * B;
        if (k == n) break;

        /* Update binomial: B(k+1, m-1) from B(k, m). */
        unsigned int m     = (unsigned int)(n - k);          /* current m */
        unsigned int numB  = (unsigned int)(k + 1) * m;
        unsigned int denB  = (unsigned int)(k - m + 2) *
                            (unsigned int)(k - m + 1);
        B = mul_small(B, numB);  B /= denB;

        /* Update C to the next k. */
        unsigned int kk = (unsigned int)(k + 1);
        C = mul_small(C, 4 * kk);   C /= kk;
        C = mul_small(C, 4 * kk - 1); C /= kk;
        C = mul_small(C, 4 * kk - 2); C /= kk;
        C = mul_small(C, 4 * kk - 3); C /= kk;
    }

    return sum;
}
