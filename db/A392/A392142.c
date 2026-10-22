/*
 * A392142  Starting with a(1) = 1, each term is obtained by doubling the
 *          previous term and then deleting repeated digits, keeping only the
 *          first occurrence of each digit.
 * https://oeis.org/A392142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A137564(2*a(n-1)), a(1)=1, periodic with period 10 after
 *            n=21. A137564 keeps only the first occurrence of each decimal
 *            digit from left to right
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   dedup_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/dedup_digits.h"

/* Sequence function. */
seq_t a(idx_t n)
{
    /* Cache of first 30 terms (up to the start of the period). */
    static seq_t cache[30] = {0};
    static bool init = false;

    if (!init) {
        /* Initialise first term. */
        cache[0] = 1;
        /* Compute up to term 30. */
        for (idx_t i = 2; i <= 30; ++i) {
            seq_t prev = cache[i - 2];          /* a(i-1) */
            seq_t doubled = prev * 2;           /* 2*a(i-1) */
            cache[i - 1] = dedup_digits(doubled); /* a(i) */
        }
        init = true;
    }

    if (n <= 30) {
        return cache[n - 1];
    }
    /* Period 10 starting at n = 21. Map any larger n into that period. */
    idx_t offset = (n - 21) % 10 + 21;          /* offset in [21,30] */
    return cache[offset - 1];
}
