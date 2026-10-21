/*
 * A289285  Starting with a(1) = 1, a(n) = smallest nonnegative integer not
 *          yet in the sequence such that the last digit of a(n-1) plus the
 *          first digit of a(n) is equal to 7. The digit 0 is not allowed.
 * https://oeis.org/A289285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1. For n>1, a(n) is the smallest unused non‑negative integer that
 * contains no digit 0 and satisfies
 *   (last digit of a(n-1)) + (first digit of a(n)) = 7.
 * The sequence stops when no such integer can be found. */

#include <stdbool.h>
#include <stddef.h>

/* Capacity sufficient for the finite sequence (≥ 43 terms). */
#define MAX_SEQ_LEN 100

/* Static storage for the sequence and its current length. */
static seq_t seq[MAX_SEQ_LEN];
static size_t seq_len = 0;

/* Check whether the decimal representation of x contains the digit 0. */
static bool has_zero_digit(seq_t x)
{
    if (x == (seq_t)0) return true;
    while (x > (seq_t)0) {
        if (x % (seq_t)10 == (seq_t)0) return true;
        x /= (seq_t)10;
    }
    return false;
}

/* Return the first (most significant) decimal digit of x. */
static int first_digit(seq_t x)
{
    while (x >= (seq_t)10) x /= (seq_t)10;
    return (int)x;
}

/* As seq_len is small, a linear scan is adequate. */
static bool in_sequence(seq_t v)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq[i] == v) return true;
    return false;
}

/* Main function required by the evaluation harness. */
seq_t a(idx_t n)
{
    /* Initialise the first term lazily. */
    if (seq_len == 0) {
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* If the requested index is already known, return it. */
    if ((idx_t)seq_len >= n && n >= 1)
        return seq[(size_t)(n - 1)];

    /* Grow the sequence until we either reach the requested position
     * or cannot find a suitable next term. */
    while ((idx_t)seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        seq_t last_digit = prev % (seq_t)10;
        int required_first = 7 - (int)last_digit;

        /* If the required first digit is out of range, the sequence ends. */
        if (required_first < 1 || required_first > 9) break;

        seq_t cand = (seq_t)required_first;   /* First candidate with the needed first digit. */
        bool found = false;

        /* Search for the smallest admissible candidate. */
        for (;;) {
            if (first_digit(cand) != required_first) { ++cand; continue; }
            if (has_zero_digit(cand))                  { ++cand; continue; }
            if (in_sequence(cand))                     { ++cand; continue; }
            found = true;
            break;
        }

        if (!found) break;          /* No admissible candidate, sequence ends. */

        if (seq_len >= MAX_SEQ_LEN) /* Safety guard against overruns. */
            break;

        seq[seq_len++] = cand;     /* Append the new term. */
    }

    /* Return the requested term if available, otherwise 0. */
    if ((idx_t)seq_len >= n && n >= 1)
        return seq[(size_t)(n - 1)];
    return (seq_t)0;
}
