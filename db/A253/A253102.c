/*
 * A253102  a(n) = A071053(n)^3.
 * https://oeis.org/A253102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A071053(n)^3 * A071053(n) is the number of odd
 *            coefficients in (1+x+x^2)^n. * We compute the parity polynomial
 *            of (1+x+x^2)^n modulo 2 using * exponentiation by squaring with
 *            XOR convolution, then count the * number of 1-bits. The result
 *            is its cube. The algorithm runs in * O(n / 64) time...
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* XOR the polynomial `poly` with `poly << shift` over GF(2). */
static inline void xor_shift(uint64_t *poly, const uint64_t *src,
                             size_t words, size_t shift)
{
    size_t wshift = shift >> 6;          /* shift in whole 64‑bit words */
    unsigned bshift = shift & 63;        /* shift inside a word */
    if (wshift >= words) return;

    if (bshift == 0) {
        /* Simple word‑wise XOR. */
        for (size_t i = words; i-- > wshift; ) {
            poly[i] ^= src[i - wshift];
        }
    } else {
        /* General shift with carry across word boundaries. */
        for (size_t i = words; i-- > wshift + 1; ) {
            uint64_t val = (src[i - wshift] << bshift)
                         | (src[i - wshift - 1] >> (64 - bshift));
            poly[i] ^= val;
        }
        /* The lowest affected word. */
        poly[wshift] ^= (src[0] << bshift);
    }
}

seq_t a(idx_t n)
{
    /* If n is very small, just handle directly. */
    if (n == 0) return (seq_t)1;

    /* Maximum degree of the polynomial is 2*n. */
    size_t max_deg = (size_t)(2 * n);
    size_t words = (max_deg >> 6) + 1;   /* number of 64‑bit words needed */

    uint64_t *poly = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *tmp  = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!poly || !tmp) {
        /* If memory allocation fails, return 0; this should not happen. */
        free(poly);
        free(tmp);
        return (seq_t)0;
    }

    poly[0] = 1ULL;          /* initial polynomial = 1 */

    size_t shift = 1;         /* current power of two in exponent */
    idx_t rem = n;
    while (rem) {
        if (rem & 1) {
            /* Multiply current polynomial by (1 + x^shift + x^{2*shift}) modulo 2. */
            memcpy(tmp, poly, words * sizeof(uint64_t));
            xor_shift(poly, tmp, words, shift);          /* * x^shift term */
            xor_shift(poly, tmp, words, shift << 1);    /* * x^{2*shift} term */
        }
        shift <<= 1;
        rem >>= 1;
    }

    /* Count set bits – number of odd coefficients. */
    seq_t odd = 0;
    for (size_t i = 0; i < words; ++i) {
        odd += (seq_t)__builtin_popcountll(poly[i]);
    }

    free(poly);
    free(tmp);

    /* a(n) = odd^3 */
    seq_t cube = odd * odd * odd;
    return cube;
}
