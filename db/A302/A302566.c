/*
 * A302566  a(n) is the least positive integer not yet in the sequence in
 *          which the largest digit of a(n-2) appears among its digits, and
 *          a(n) cannot share digits with a(n-1); a(1) = 1, a(2) = 2.
 * https://oeis.org/A302566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least positive integer not yet in the sequence
   such that its decimal digits contain the largest digit of a(n-2)
   and share no digit with a(n-1);
   a(1)=1, a(2)=2.
   The sequence is generated on demand and cached.
   Sequence values are stored in static arrays of fixed maximum
   size.  This implementation works for all supported seq_t types
   (integral or arbitrary‑precision C++ classes). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef MAX_TERMS
#  define MAX_TERMS 20000
#endif

/* Indexed from 0 internally; the external index n is 1‑based. */
static seq_t seq_arr[MAX_TERMS];
static uint16_t mask_arr[MAX_TERMS];
static seq_t used_arr[MAX_TERMS];

static idx_t seq_len = 0;   /* number of sequence terms stored */
static idx_t used_len = 0;  /* number of values stored in used_arr */

/* largest decimal digit of v (v>0) */
static int max_digit(seq_t v) {
    int m = 0;
    while (v > 0) {
        int d = (int)(v % 10);
        if (d > m) m = d;
        v /= 10;
    }
    return m;
}

/* bit mask of decimal digits of v */
static uint16_t digit_mask(seq_t v) {
    uint16_t m = 0;
    while (v > 0) {
        int d = (int)(v % 10);
        m |= 1u << d;
        v /= 10;
    }
    return m;
}

/* binary search: true if x is already used */
static bool used_contains(seq_t x) {
    size_t lo = 0, hi = (size_t)used_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (used_arr[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < (size_t)used_len && used_arr[lo] == x);
}

/* insert x into used_arr keeping it sorted */
static void used_insert(seq_t x) {
    size_t lo = 0, hi = (size_t)used_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (used_arr[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    size_t pos = lo;
    /* shift elements to make room */
    for (size_t i = used_len; i > pos; --i)
        used_arr[i] = used_arr[i - 1];
    used_arr[pos] = x;
    ++used_len;
}

/* append new term to sequence arrays */
static void append_seq_value(seq_t val, uint16_t mask) {
    if (seq_len >= MAX_TERMS) return; /* safety: should not happen for n ≤ MAX_TERMS */
    seq_arr[(size_t)seq_len] = val;
    mask_arr[(size_t)seq_len] = mask;
    used_insert(val);
    ++seq_len;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n <= 0) return 0;          /* offset is 1, undefined for ≤0 */

    /* initialise first two terms if not already done */
    if (seq_len == 0) {
        seq_t val1 = 1;
        seq_t val2 = 2;
        append_seq_value(val1, digit_mask(val1));
        append_seq_value(val2, digit_mask(val2));
    }

    /* generate additional terms up to n */
    while (seq_len < n) {
        seq_t val;
        uint16_t mask;
        /* index of new term in 0‑based system */
        size_t idx0 = (size_t)seq_len;

        if (idx0 == 0) {          /* a(1) */
            val = 1;
            mask = digit_mask(val);
        } else if (idx0 == 1) {   /* a(2) */
            val = 2;
            mask = digit_mask(val);
        } else {
            /* largest digit of a(n-2) */
            int D = max_digit(seq_arr[(size_t)(idx0 - 2)]);
            uint16_t mask_prev = mask_arr[(size_t)(idx0 - 1)];
            seq_t cand = 1;
            while (true) {
                if (used_contains(cand)) {
                    ++cand;
                    continue;
                }
                uint16_t cand_mask = digit_mask(cand);
                if ((cand_mask & mask_prev) == 0 &&
                    (cand_mask & (1u << D)) != 0) {
                    val = cand;
                    mask = cand_mask;
                    break;
                }
                ++cand;
            }
        }

        append_seq_value(val, mask);
    }

    return seq_arr[(size_t)(n - 1)];
}
