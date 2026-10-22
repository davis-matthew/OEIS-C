/*
 * A321071  Twice the Thue-Morse constant (A014571).
 * https://oeis.org/A321071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   mul10_uint32, thue_tmorse_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A321071 – Decimal digit of twice the Thue–Morse constant
 *  a(n) = the n‑th decimal digit of the number
 *     2 * Σ_{k≥0} t(k)/2^k
 *  where t(k) = 0 if popcount(k) even, 1 otherwise.
 *  The algorithm multiplies the binary fraction by 10 repeatedly,
 *  extracting one decimal digit per iteration.  A small big‑integer
 *  representation (array of 32‑bit words) is used; the routine
 *  is O(n) and works for any seq_t type (8,16,32,64,128 or arbitrary‐
 *  precision).  No external libraries or printf calls are needed. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul10_uint32.h"
#include "../utils/thue_tmorse_bit.h"

seq_t a(idx_t n)
{
    /* choose L such that 2^L > 10^(n+1) – 4*(n+1) is a safe over‑estimate */
    idx_t L = 4 * (n + 1);
    size_t words = (L >> 5) + 3;          /* little extra space for carries */

    uint32_t *Q = (uint32_t *)calloc(words, sizeof(uint32_t));
    if (!Q) return (seq_t)0;              /* out of memory – return 0 */

    /* build integer N = floor( s * 2^L ) where s = Σ t(k)/2^k */
    for (idx_t k = 0; k <= L; ++k) {
        if (thue_tmorse_bit(k)) {
            idx_t pos  = L - k;           /* bit position in N (0‑based) */
            size_t wi  = pos >> 5;        /* word index                */
            uint32_t b = 1U << (pos & 31);/* bit mask                   */
            Q[wi] |= b;
        }
    }

    /* parameters for extraction of decimal digits */
    size_t shift      = L & 31;            /* residual bits in high word */
    size_t word_shift = L >> 5;            /* whole‑word shift */
    seq_t digit = 0;

    for (idx_t iter = 0; iter <= n; ++iter) {
        /* multiply current remainder by 10 */
        mul10_uint32(Q, words);

        /* obtain next decimal digit (low 4 bits of Q / 2^L) */
        if (shift == 0) {
            /* L is multiple of 32: digit lives in word_shift (low 4 bits) */
            if (word_shift < words)
                digit = Q[word_shift] & 0xFULL;
            else
                digit = 0;
        } else {
            /* digit may cross a word boundary */
            uint64_t high = 0;
            if (word_shift + 1 < words)
                high = Q[word_shift + 1];
            uint64_t combined = (high << (32 - shift)) | (Q[word_shift] >> shift);
            digit = combined & 0xFULL;
        }

        /* remainder Q = Q mod 2^L for next iteration */
        if (shift == 0) {
            for (size_t w = word_shift; w < words; ++w)
                Q[w] = 0;
        } else {
            uint32_t mask = (1U << shift) - 1U;
            Q[word_shift] &= mask;
            for (size_t w = word_shift + 1; w < words; ++w)
                Q[w] = 0;
        }
    }

    free(Q);
    return (seq_t)digit;
}
