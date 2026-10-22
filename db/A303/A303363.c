/*
 * A303363  Number of ways to write n as a*(a+1)/2 + b*(b+1)/2 + 2^c + 2^d,
 *          where a,b,c,d are nonnegative integers with a <= b, c <= d and
 *          2|c.
 * https://oeis.org/A303363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 1472, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count representations n = a*(a+1)/2 + b*(b+1)/2 + 2^c + 2^d
   with a,b,c,d non‑negative integers, a <= b, c <= d, c even.
   a(1) is the first term of the OEIS sequence. */

#include <stdbool.h>

/* compute A303363(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety for zero index */

    /* precompute powers of two up to n */
    idx_t maxPowExp = 0;
    seq_t pow2[64];
    pow2[0] = (seq_t)1;
    while (pow2[maxPowExp] <= (seq_t)n) {  /* grow until > n */
        if (pow2[maxPowExp] > (seq_t)n / 2) {
            break;                        /* avoid overflow on next shift */
        }
        ++maxPowExp;
        pow2[maxPowExp] = pow2[maxPowExp - 1] << 1;
    }

    /* find maximum a such that T(a) <= n, where T(a)=a*(a+1)/2 */
    idx_t maxA = 0;
    while (true) {
        seq_t tri = (seq_t)maxA * (maxA + 1) / 2;
        if (tri > (seq_t)n) break;
        ++maxA;                             /* next a will fail, so stop */
    }

    seq_t result = (seq_t)0;

    for (idx_t a = 0; a <= maxA; ++a) {
        seq_t tA = (seq_t)a * (a + 1) / 2;
        for (idx_t b = a; b <= maxA; ++b) {
            seq_t tB = (seq_t)b * (b + 1) / 2;
            seq_t sumAB = tA + tB;
            if (sumAB > (seq_t)n) break;     /* further b will only increase */
            seq_t r = (seq_t)n - sumAB;      /* remaining part to be 2^c+2^d */

            /* iterate over even c */
            for (idx_t cExp = 0; cExp <= maxPowExp; cExp += 2) {
                seq_t twoC = pow2[cExp];
                if (twoC > r) break;
                seq_t rest = r - twoC;
                if (rest == 0) continue;     /* cannot be 2^d with d>=0 */
                if ((rest & (rest - 1)) != 0) continue;  /* not a power of two */

                /* find d >= c such that 2^d == rest */
                for (idx_t dExp = cExp; dExp <= maxPowExp; ++dExp) {
                    if (pow2[dExp] == rest) {
                        ++result;
                        break;                 /* unique (c,d) found */
                    }
                }
            }
        }
    }

    return result;
}
