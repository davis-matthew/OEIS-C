/*
 * A348503  a(n) = gcd(sigma(n), usigma(n)), where sigma is the sum of
 *          divisors function, A000203, and usigma is the unitary sigma,
 *          A034448.
 * https://oeis.org/A348503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(σ(n), σ*(n)) where σ(n) is the sum‑of‑divisors and
 *            σ*(n) is the unitary sum‑of‑divisors
 * Range:     u8 n <= 137, u16 n <= 8692, u32 n <= 8692, u64 n <= 8692,
 *            u128 n <= 8692, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Compute the greatest common divisor of two seq_t values (no
   external helper needed).  The operators +, -, *, /, % are defined
   for seq_t in the target build. */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                /* safety, not part of the offset 1 sequence */
        return 0;

    /* Working copy of n for factorisation.  n is uint64_t (idx_t). */
    uint64_t u = (uint64_t)n;

    seq_t sigma   = 1;         /* product over (1 + p + p² + … + pᵉ) */
    seq_t usigma  = 1;         /* product over (pᵉ + 1) */

    uint64_t d = 2;
    while (d <= u / d) {      /* loop over potential prime divisors */
        if (u % d == 0) {
            int exp = 0;
            while (u % d == 0) {
                u /= d;
                exp++;
            }

            seq_t pow = 1;    /* pᵉ */
            seq_t sum = 1;    /* 1 + p + … + pᵉ */

            for (int i = 0; i < exp; ++i) {
                pow *= (seq_t)d;
                sum += pow;
            }

            sigma  *= sum;
            usigma *= (pow + 1);
        }
        d++;
    }

    if (u > 1) {              /* u is now a prime factor > sqrt(original n) */
        seq_t p = (seq_t)u;
        sigma  *= (p + 1);
        usigma *= (p + 1);
    }

    return gcd_seq(sigma, usigma);
}
