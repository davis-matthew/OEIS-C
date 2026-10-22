/*
 * A346678  Positive numbers whose squares end in exactly two identical
 *          digits.
 * https://oeis.org/A346678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ends_with_two_identical_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: Positive numbers whose squares end in exactly two identical digits.
 * Computation: The sequence satisfies the recurrence a(n+63) = a(n)+500.
 * The first 63 terms are found by brute‑force scanning of natural numbers,
 * checking that the square ends with exactly two equal digits (00 or 44)
 * and not with a longer run of the same digit.
 */
#include <stdbool.h>
#include "../utils/ends_with_two_identical_digits.h"

static bool initialized = false;
static seq_t base[63];

/* Populate the first 63 terms on first use. */
static void init_base(void) {
    idx_t count = 0;
    for (idx_t m = 1; count < 63; ++m) {
        if (ends_with_two_identical_digits(m)) {
            base[count++] = (seq_t)m;
        }
    }
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init_base();
    /* Offset 1: a(1) corresponds to base[0]. */
    idx_t idx = n - 1u;
    seq_t period_term = base[idx % 63u];
    seq_t add = (seq_t)(idx / 63u) * (seq_t)500u;
    return period_term + add;
}
