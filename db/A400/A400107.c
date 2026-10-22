/*
 * A400107  Decimal expansion of Sum_{m >= 0, m not a binary self number}
 *          2^(-m).
 * https://oeis.org/A400107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9095, u16 n <= 9095, u32 n <= 9095, u64 n <= 9095,
 *            u128 n <= 9095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the nth digit in the decimal expansion of the constant  
 *     C = sum_{m>=0, m not a binary self number} 2^{-m}
 * The sequence is indexed starting at n = 1 (a(1) = 1).
 * The algorithm constructs the binary expansion of the complement
 * of binary self numbers up to a suitable limit, then converts
 * the resulting binary fraction to decimal digits by repeated
 * multiplication by 10 and truncation.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* integer part */
    if (n <= 1) {
        /* n==1 -> integer part is 1 because 0 is not a binary self number */
        return (seq_t)1;
    }

    /* number of fractional digits needed */
    idx_t frac_digits = n - 1;

    /* choose M such that 2^{-M} < 10^{-(frac_digits+1)}.
       5 * (frac_digits + 5) is a safe upper bound. */
    idx_t M = (frac_digits + 5) * 5;     /* safety margin */

    /* --- mark non-self (complement of binary self numbers) up to M --- */
    uint8_t *nonself = (uint8_t *)calloc((size_t)(M + 1), sizeof(uint8_t));
    if (!nonself) return (seq_t)0;       /* allocation failed */

    for (idx_t i = 0; i <= M; ++i) {
        /* g2(i) = i + popcount(i)  */
        idx_t pos = i + __builtin_popcountll(i);
        if (pos <= M) {
            nonself[pos] = 1;           /* non-self number */
        }
    }

    /* binary fraction numerator occupies bits 0..M-1 (fractional part)
       and bit M (integer part).  We skip bit M here. */
    idx_t words = (M >> 5) + 3; /* +3 for safety, 32 bits per word */
    uint32_t *rem = (uint32_t *)calloc((size_t)words, sizeof(uint32_t));
    if (!rem) {
        free(nonself);
        return (seq_t)0;
    }

    /* set bits for fractional part (m >= 1) */
    for (idx_t m = 1; m <= M; ++m) {
        if (nonself[m]) {
            idx_t pos = M - m;                /* position to set */
            rem[pos >> 5] |= (1U << (pos & 31));
        }
    }

    /* --- generate fractional digits --- */
    for (idx_t d = 1; d <= frac_digits; ++d) {
        /* rem = rem * 10 */
        uint32_t carry = 0;
        for (idx_t i = 0; i < words; ++i) {
            uint64_t tmp = (uint64_t)rem[i] * 10 + carry;
            rem[i] = (uint32_t)tmp;
            carry  = (uint32_t)(tmp >> 32);
        }

        /* digit = rem >> M   (fragment of high bits) */
        int digit = 0;
        for (int k = 0; k < 5; ++k) {     /* 5 bits enough for digit 0-9 */
            idx_t pos   = M + k;
            idx_t w     = pos >> 5;
            int   bit   = (rem[w] >> (pos & 31)) & 1;
            digit |= bit << k;
        }

        /* if requesting this digit, return it */
        if (d == frac_digits) {
            free(rem);
            free(nonself);
            return (seq_t)digit;
        }

        /* keep only bits < M (i.e., rem = rem % 2^M) */
        idx_t wshift = M >> 5;
        int   offset = M & 31;

        /* zero higher words */
        for (idx_t i = wshift + 1; i < words; ++i) {
            rem[i] = 0;
        }

        /* mask the top word */
        if (offset == 0) {
            rem[wshift] = 0;
        } else {
            uint32_t mask = (1U << offset) - 1;
            rem[wshift] &= mask;
        }
    }

    /* should not reach here */
    free(rem);
    free(nonself);
    return (seq_t)0;
}
