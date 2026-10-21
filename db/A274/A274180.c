/*
 * A274180  a(n) = Sum_{k=0..4n} (A035343(n,k) mod 2) * 2^k.
 * https://oeis.org/A274180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=0..4n} (A035343(n,k) mod 2) * 2^k
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           where A035343(n,k) is the coefficient of x^k in (1+x+x^2+x^3+x^4)^n   */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* number of bits needed for the polynomial: 4*n + 1 */
    idx_t bits_len_idx = n * 4 + 1;
    size_t bits_len = (size_t)bits_len_idx;
    size_t words = (bits_len + 63) / 64;   /* number of 64‑bit words */

    /* allocate coefficient buffers */
    uint64_t *curr = (uint64_t *)malloc(words * sizeof(uint64_t));
    uint64_t *next = (uint64_t *)malloc(words * sizeof(uint64_t));
    if (!curr || !next) {
        /* allocation failure – return 0 */
        if (curr) free(curr);
        if (next) free(next);
        return 0;
    }

    /* initialise to (1+x+x^2+x^3+x^4)^0 */
    for (size_t i = 0; i < words; ++i) {
        curr[i] = 0;
        next[i] = 0;
    }
    curr[0] = 1;

    /* repeated convolution with the base polynomial [1,1,1,1,1] modulo 2 */
    for (idx_t step = 1; step <= n; ++step) {
        /* start with shift 0 (copy of previous coefficients) */
        for (size_t i = 0; i < words; ++i) {
            next[i] = curr[i];
        }

        /* apply shifts 1..4 and xor */
        for (int s = 1; s <= 4; ++s) {
            size_t shift      = (size_t)s;
            size_t word_shift = shift / 64;
            unsigned bit_shift = shift % 64;

            for (size_t i = 0; i < words; ++i) {
                uint64_t low  = 0;
                uint64_t high = 0;

                if (i >= word_shift) {
                    low = curr[i - word_shift] << bit_shift;
                }
                if (bit_shift != 0 && i > word_shift) {
                    high = curr[i - word_shift - 1] >> (64 - bit_shift);
                }
                next[i] ^= low | high;
            }
        }

        /* swap buffers for next iteration */
        uint64_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* build the resulting integer from the most significant bit to the least */
    seq_t res = 0;
    for (long k = (long)(bits_len - 1); k >= 0; --k) {
        size_t word = k / 64;
        unsigned bit = k % 64;
        res <<= 1;
        if (curr[word] & ((uint64_t)1 << bit)) {
            res |= 1;
        }
    }

    free(curr);
    free(next);
    return res;
}
