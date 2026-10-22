/*
 * A301150  Coordination sequence T1 for Zeolite Code OWE.
 * https://oeis.org/A301150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-2) + 9*a(n-3) - 11*a(n-4) + 12*a(n-5) -
 *            12*a(n-6) + 13*a(n-7) - 15*a(n-8) + 18*a(n-9) - 20*a(n-10) +
 *            20*a(n-11) - 18*a(n-12) + 15*a(n-13) - 13*a(n-14) + 12*a(n-15) -
 *            12*a(n-16) + 11*a(n-17) - 9*a(n-18) + 6*a(n-19) - 3*a(n-20) +
 *            a(n-21) (for n > 21), with a(0)=1, a(1)=4, a(...
 * Range:     u8 n <= 11, u16 n <= 178, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t base_terms[22] = {
    1ULL, 4ULL, 9ULL, 18ULL, 35ULL, 57ULL, 76ULL, 96ULL, 128ULL, 172ULL,
    216ULL, 252ULL, 290ULL, 343ULL, 408ULL, 471ULL, 528ULL, 591ULL,
    665ULL, 745ULL, 828ULL, 912ULL
};

static const int coeff[21] = {
    3, -6, 9, -11, 12, -12, 13, -15, 18,
    -20, 20, -18, 15, -13, 12, -12, 11, -9, 6, -3, 1
};

seq_t a(idx_t n)
{
    /* Terms 0..21 are known explicitly. */
    if (n < 22)
        return base_terms[n];

    /* Maintain a circular buffer of the last 22 computed terms
       (indices modulo 22). Initialize it with the base terms. */
    seq_t buf[22];
    for (int i = 0; i < 22; ++i)
        buf[i] = base_terms[i];

    /* Compute terms from 22 up to n */
    for (idx_t i = 22; i <= n; ++i) {
        idx_t idx = i % 22;          /* position to store a(i) */
        seq_t pos = 0;                /* sum of positive coefficient terms */
        seq_t neg = 0;                /* sum of absolute value of negative coeff terms */

        for (int k = 1; k <= 21; ++k) {
            idx_t prev = (i - k) % 22;             /* a(i-k) */
            seq_t val  = buf[prev];
            int  c     = coeff[k - 1];
            seq_t prod = val * (seq_t)(c >= 0 ? c : -c);

            if (c >= 0)
                pos += prod;
            else
                neg += prod;
        }
        buf[idx] = pos - neg;   /* pos >= neg for the given sequence */
    }

    return buf[n % 22];
}
