/*
 * A272264  Numbers that become a different number when flipped upside down.
 * https://oeis.org/A272264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1141, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that become a different number when flipped upside down.
   The function builds the sequence in increasing order by testing each
   candidate number for digit validity, leading‑zero avoidance after flipping,
   and inequivalence to its flipped counterpart. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Mapping from the original digit to the flipped digit.
   -1 means the digit is forbidden for this sequence.     */
static const int digit_map[10] = {
    0,  1, -1, -1, -1, -1,
    9, -1, 8,  6
};

/* Return true if x is a valid term of the sequence. */
static bool valid_number(seq_t x)
{
    /* Reject if the last digit would become 0 after flipping,
       which would create a leading zero. */
    int last_digit = (int)(x % 10);
    if (digit_map[last_digit] == 0)
        return false;

    seq_t flipped = 0;
    seq_t y = x;
    while (y > 0) {
        int d = (int)(y % 10);
        int m = digit_map[d];
        if (m == -1)            /* digit not allowed */
            return false;
        flipped = flipped * 10 + (seq_t)m;
        y /= 10;
    }
    /* Reject numbers that stay the same when flipped. */
    return flipped != x;
}

/* Storage for the sequence. */
static seq_t *seq_vals = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static idx_t next_candidate = 1;

seq_t a(idx_t n)
{
    if (n == 0) /* Undefined for 0; return 0 as a placeholder. */
        return (seq_t)0;

    /* Generate terms until we have the nth term. */
    while (seq_len < n) {
        /* Scan candidates starting from the last checked number. */
        for (idx_t cand = next_candidate; ; ++cand) {
            seq_t val = (seq_t)cand;
            if (!valid_number(val))
                continue;

            /* Ensure capacity in the array. */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                seq_t *tmp =
                    (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0; /* Allocation failed; return 0. */
                seq_vals = tmp;
                seq_cap = new_cap;
            }

            seq_vals[seq_len++] = val;
            next_candidate = cand + 1;
            break; /* Move to generate the next term. */
        }
    }

    return seq_vals[n - 1];
}
