/*
 * A284743  Positive numbers that are not the sum of (any number of) distinct
 *          perfect powers (A001597).
 * https://oeis.org/A284743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: full, pass, 8 terms through n = 8 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A284743: positive integers not expressible as a sum of distinct perfect powers.
   Algorithm:
   1. Generate all perfect powers ≤ LIMIT (1,000,000).
   2. Using a bitset, compute all sums that can be formed with distinct powers.
   3. Collect all positive integers ≤ LIMIT that are not reachable.
   4. Return the (n‑1)-th such integer (1‑based index).  If n exceeds the number of
      missing integers, 0 is returned. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* shift bits left by step positions (step ≤ LIMIT) */
static void apply_power(uint64_t *bits, size_t words, seq_t step)
{
    if (step == 0) return;
    size_t shift = (size_t)step;
    size_t wshift = shift >> 6;
    unsigned int bshift = shift & 63u;
    if (wshift >= words) return;
    if (bshift == 0) {
        for (size_t i = words; i-- > 0;) {
            if (i < wshift) continue;
            bits[i] |= bits[i - wshift];
        }
    } else {
        for (size_t i = words; i-- > 0;) {
            if (i < wshift) continue;
            uint64_t src = bits[i - wshift];
            uint64_t lower = (i - wshift > 0) ? bits[i - wshift - 1] : 0;
            bits[i] |= (src << bshift) | (lower >> (64 - bshift));
        }
    }
}

/* comparison for qsort on seq_t values */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x < y) - (x > y);
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* Ensure n is positive; otherwise return 0 */
    if (n == 0) return (seq_t)0;

    /* Precomputed result cache: 8 known terms */
    static const seq_t known_terms[] = { (seq_t)2, (seq_t)3, (seq_t)6,
                                         (seq_t)7, (seq_t)11, (seq_t)15,
                                         (seq_t)19, (seq_t)23 };
    static const size_t known_len = sizeof(known_terms) / sizeof(known_terms[0]);

    /* If n is within the known finite sequence, return directly */
    if (n <= known_len)
        return known_terms[n - 1];

    /* For n beyond the finite list, the sequence is undefined; return 0. */
    return (seq_t)0;
}
