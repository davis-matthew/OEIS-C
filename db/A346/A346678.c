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
 * Validated: standard, pass, 75 terms through n = 986 (2026-11-01)
 * Helpers:   none
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

/* Helper to test if m^2 ends with exactly two equal digits. */
static bool ends_with_two_identical_digits(idx_t m) {
    seq_t sq = (seq_t)m * (seq_t)m;
    seq_t last_two = sq % 100u;          /* last two digits */
    seq_t d = last_two / 10u;            /* tens digit */
    seq_t o = last_two % 10u;            /* units digit */
    if (d != o) return false;            /* they must be equal */
    seq_t last_three = sq % 1000u;       /* last three digits */
    seq_t third = (last_three / 100u) % 10u; /* hundreds digit */
    return third != d;                   /* not a longer run */
}

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
