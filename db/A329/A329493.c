/*
 * A329493  (Sum of digits of (2^n - 11)) - n.
 * https://oeis.org/A329493
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = digit_sum(2^n - 11) - n
 * Range:     i8 n <= 281, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Computes the OEIS sequence A329493 for n ≥ 4.
 * Works for any seq_t and idx_t types.
 * Uses decimal-base representation in chunks of 1e9.
 * Works even when seq_t cannot hold 2^n.
 */
seq_t a(idx_t n)
{
    /* 2^n is never less than 11 for n ≥ 4, so the value is positive. */
    if (n < 4) {
        /* For indices below the offset we can return 0 or any value;
           it is harmless because the sequence is defined only for n ≥ 4. */
        return 0;
    }

    /* Estimate the number of decimal digits of 2^n.  */
    const double log10_2 = 0.3010299956639811952137389;
    double d = (double)n * log10_2;
    size_t needed_digits = (size_t)(d) + 3;          /* extra padding */

    /* Each chunk holds 9 decimal digits (base = 1e9). */
    const uint32_t BASE = 1000000000u;
    size_t cap_chunks = needed_digits / 9 + 2;       /* a few spare chunks */

    /* Allocate the array that stores the number in base BASE. */
    uint32_t *dig = (uint32_t *)malloc(cap_chunks * sizeof(uint32_t));
    if (!dig) return 0;                              /* allocation failed */

    /* Start with the value 1. */
    dig[0] = 1;
    size_t len = 1;

    /* Compute 2^n by repeated doubling. */
    for (idx_t i = 0; i < n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t t = (uint64_t)dig[j] * 2 + carry;
            dig[j] = (uint32_t)(t % BASE);
            carry = t / BASE;
        }
        if (carry) {
            dig[len++] = (uint32_t)carry;
        }
    }

    /* Subtract 11 from the big integer. */
    int64_t borrow = 11;
    size_t j = 0;
    while (borrow && j < len) {
        int64_t diff = (int64_t)dig[j] - borrow;
        if (diff < 0) {
            dig[j] = (uint32_t)(diff + BASE);
            borrow = 1;
        } else {
            dig[j] = (uint32_t)diff;
            borrow = 0;
        }
        ++j;
    }
    /* 2^n ≥ 11 for n≥4, so borrow should be 0 here. */

    /* Sum the decimal digits of the resulting number. */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        uint32_t v = dig[i];
        if (i == len - 1 && v == 0) continue; /* skip leading zero chunk */
        while (v > 0) {
            sum += (seq_t)(v % 10u);
            v /= 10u;
        }
    }

    /* Free the temporary array. */
    free(dig);

    /* Final result: digit sum minus the index n. */
    return sum - (seq_t)n;
}
