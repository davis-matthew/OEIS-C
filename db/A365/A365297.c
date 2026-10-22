/*
 * A365297  a(n) is the smallest number k such that k*n is a number whose
 *          prime factorization exponents are all powers of 2 (A138302).
 * https://oeis.org/A365297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} p^{2^{⌈log₂(e)⌉} – e}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>    /* for the standard integer types */

seq_t a(idx_t n) {
    idx_t rem = n;           /* remaining factor to be processed      */
    seq_t prod = (seq_t)1;   /* accumulator for the result            */

    /* factorize n by trial division */
    for (idx_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            idx_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }

            /* compute the smallest power of two ≥ e */
            idx_t tw = 1;
            while (tw < e) {
                tw <<= 1;                /* tw = 2^ceil(log₂(e))         */
            }
            idx_t diff = tw - e;          /* exponent to multiply by p    */

            for (idx_t i = 0; i < diff; ++i) {
                prod *= (seq_t)p;         /* multiply p^diff into product */
            }
        }
    }

    /* if a prime > sqrt(original n) remains, its exponent is 1
       → ceil(log₂(1)) = 0, diff = 0 → no change to prod            */
    return prod;
}
