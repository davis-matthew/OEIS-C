/*
 * A342919  a(n) = A003415(n) / gcd(A001615(n), A003415(n)), where A001615 is
 *          Dedekind psi, and A003415 is the arithmetic derivative of n.
 * https://oeis.org/A342919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(n) / gcd(A001615(n), A003415(n))
 * Range:     u8 n <= 95, u16 n <= 9804, u32 n <= 9804, u64 n <= 9804,
 *            u128 n <= 9804, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    seq_t n_seq = (seq_t)n;
    seq_t d = (seq_t)0;
    seq_t psi = (seq_t)1;

    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* psi part:  p^(e-1)*(p+1) */
            seq_t pow_term = (seq_t)1;
            for (int i = 0; i < e - 1; ++i) pow_term *= (seq_t)p;
            seq_t termPsi = pow_term * ((seq_t)p + (seq_t)1);
            psi *= termPsi;

            /* d part:  e * (n / p) */
            seq_t termD = (seq_t)e * (n_seq / (seq_t)p);
            d += termD;
        }
    }

    if (temp > 1) {           /* remaining prime factor */
        idx_t p = temp;
        int e = 1;

        seq_t pow_term = (seq_t)1;          /* because e-1 == 0 */
        seq_t termPsi = pow_term * ((seq_t)p + (seq_t)1);
        psi *= termPsi;

        seq_t termD = (seq_t)e * (n_seq / (seq_t)p);
        d += termD;
    }

    seq_t g = gcd(d, psi);
    return d / g;
}
