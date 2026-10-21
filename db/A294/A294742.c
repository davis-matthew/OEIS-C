/*
 * A294742  Numbers that are the sum of 5 nonzero squares in exactly 8 ways.
 * https://oeis.org/A294742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: returns the nth integer that can be represented as the sum of
 * five non‑zero squares in exactly eight distinct unordered ways.
 * The sequence is finite; all values are found by an exhaustive search
 * up to the proven bound 5408.  For n beyond the number of terms,
 * the function returns 0. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_N 5408            /* proven upper bound for terms in the sequence */
#define SQ_LIMIT 73           /* 73^2 = 5329 < 5408, 74^2 = 5476 > 5408 */

/* Storage for the terms (there are only 12 such numbers) */
static seq_t term_values[20];
static size_t term_count = 0;
static bool initialized = false;

/* Helper: performs the search once and fills term_values */
static void init_terms(void)
{
    if (initialized)
        return;

    /* Pre‑compute squares of 1..SQ_LIMIT */
    uint32_t square[SQ_LIMIT];
    for (idx_t i = 0; i < SQ_LIMIT; ++i)
        square[i] = (uint32_t)(i + 1) * (i + 1);

    /* Count unordered representations (a ≤ b ≤ c ≤ d ≤ e) */
    static uint32_t rep[MAX_N + 1];
    for (idx_t a = 0; a < SQ_LIMIT; ++a) {
        uint32_t sa = square[a];
        for (idx_t b = a; b < SQ_LIMIT; ++b) {
            uint32_t s2 = sa + square[b];
            if (s2 > MAX_N) break;
            for (idx_t c = b; c < SQ_LIMIT; ++c) {
                uint32_t s3 = s2 + square[c];
                if (s3 > MAX_N) break;
                for (idx_t d = c; d < SQ_LIMIT; ++d) {
                    uint32_t s4 = s3 + square[d];
                    if (s4 > MAX_N) break;
                    for (idx_t e = d; e < SQ_LIMIT; ++e) {
                        uint32_t sum = s4 + square[e];
                        if (sum > MAX_N) break;
                        ++rep[sum];
                    }
                }
            }
        }
    }

    /* Extract all numbers with exactly 8 representations */
    term_count = 0;
    for (idx_t n = 1; n <= MAX_N; ++n) {
        if (rep[n] == 8) {
            if (term_count < 20) {          /* safety check */
                term_values[term_count++] = (seq_t)n;
            }
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init_terms();
    if (!initialized || n == 0 || n > term_count)
        return (seq_t)0;
    return term_values[n - 1];
}
