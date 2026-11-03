/*
 * A366585  Write down the positive integers. To obtain the terms of the
 *          sequence, concatenate groups of these so the first digit of each
 *          term is the number of consecutive numbers that are concatenated.
 * https://oeis.org/A366585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) computed by concatenating groups of consecutive integers
 *            where the group size equals the first digit (most‑significant
 *            digit) of the first integer in the group
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 26, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-01)
 * Helpers:   num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/num_digits0.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Return the most significant digit of a 64‑bit unsigned integer. */
static inline uint64_t msd_u64(uint64_t x) {
    while (x >= 10)
        x /= 10;
    return x;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */
    uint64_t current = 1;         /* first integer to process   */
    idx_t term_idx = 1;           /* current term number        */

    while (1) {
        uint64_t group_size = msd_u64(current);

        /* Build the term by concatenating the group of integers. */
        seq_t term_val = 0;
        for (uint64_t j = current; j < current + group_size; ++j) {
            unsigned digits = (unsigned)num_digits0(j);
            term_val = term_val * pow10(digits) + (seq_t)j;
        }

        if (term_idx == n)
            return term_val;

        current += group_size;    /* next group starts after this one */
        ++term_idx;
    }
}
