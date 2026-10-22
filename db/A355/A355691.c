/*
 * A355691  Dirichlet inverse of A320111, number of divisors of n that are not
 *          of the form 4k+2.
 * https://oeis.org/A355691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(1)=1; for n>1, a(n)= -Σ_{d|n,d<n} A320111(n/d)*a(d).
 *            Multiplicative with a(2^e) = A010892(e+2), a(p) = -2, a(p²) = 1,
 *            a(p^e)= 0 for e>2. Computes multiplicatively using the
 *            recurrence described above
 * Range:     i8 n <= 9709, i16 n <= 9709, i32 n <= 9709, i64 n <= 9709,
 *            i128 n <= 9709, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute a(2^e) for 0 <= e <= 63 (enough for uint64_t indices). */
    static bool init = false;
    static seq_t pow2_val[64];

    if (!init) {
        pow2_val[0] = 1;               /* a(1) */
        for (idx_t e = 1; e < 64; ++e) {
            seq_t sum = 0;
            /* Sum over proper divisors 2^i, i = 0 .. e-1. */
            for (idx_t i = 0; i < e; ++i) {
                /* A320111(2^(e-i)) = (e-i) for e-i>=1, and 1 for e-i==0. */
                seq_t coef = (seq_t)((e - i) == 0 ? 1 : (e - i));
                sum += coef * pow2_val[i];
            }
            pow2_val[e] = -sum;        /* a(2^e) = -sum */
        }
        init = true;
    }

    if (n == 1) {
        return 1;
    }

    seq_t result = 1;
    idx_t m = n;

    /* Factor out powers of 2. */
    idx_t e2 = 0;
    while ((m & 1ULL) == 0) {          /* m % 2 == 0 */
        ++e2;
        m >>= 1;
    }
    if (e2 > 0) {
        result *= pow2_val[e2];
        if (result == 0) {
            return 0;
        }
    }

    /* Trial division for odd prime factors up to sqrt(m). */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            seq_t local;
            if (e == 1) {
                local = -2;
            } else if (e == 2) {
                local =  1;
            } else {
                local =  0;           /* e > 2 */
            }
            result *= local;
            if (result == 0) {
                return 0;
            }
        }
    }

    /* Remaining prime factor (if any) has exponent 1. */
    if (m > 1) {
        result *= (seq_t)(-2);
    }

    return result;
}
