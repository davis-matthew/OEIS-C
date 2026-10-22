/*
 * A351626  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest positive
 *          number that has not appeared that shares a factor with both the
 *          largest and second largest value of all previous terms.
 * https://oeis.org/A351626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A351626: a(1)=1, a(2)=2; for n>2 a(n) is the smallest positive
 * number not yet used that shares a non‑trivial common factor with
 * both the largest and second‑largest values of all previous terms. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Static storage for the generated terms. */
static seq_t *seq_arr = NULL;   /* array of terms */
static size_t seq_len = 0;      /* number of terms stored */
static size_t seq_cap = 0;      /* allocated capacity */

/* Grow the array to hold at least min_cap elements. */
static inline void grow_seq_arr(size_t min_cap) {
    if (min_cap <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 4;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Return true if v has already appeared in the sequence. */
static inline bool is_used(seq_t v) {
    for (size_t i = 0; i < seq_len; ++i)
        if (seq_arr[i] == v)
            return true;
    return false;
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out of range */

    /* Ensure we have at least the first two terms. */
    if (seq_len == 0) {
        grow_seq_arr(2);
        seq_arr[0] = (seq_t)1;
        seq_arr[1] = (seq_t)2;
        seq_len = 2;
    }
    if (n <= seq_len)
        return seq_arr[n-1];

    /* Determine current largest and second largest values. */
    seq_t largest   = seq_arr[0];
    seq_t second_max = (seq_t)0;
    for (size_t i = 1; i < seq_len; ++i) {
        seq_t v = seq_arr[i];
        if (v > largest) {
            second_max = largest;
            largest    = v;
        } else if (v > second_max) {
            second_max = v;
        }
    }

    /* Generate terms until we reach index n. */
    for (; seq_len < (size_t)n; ++seq_len) {
        /* Candidate must be unused. */
        seq_t candidate = (seq_t)2;
        while (true) {
            if (is_used(candidate)) {
                ++candidate;
                continue;
            }
            seq_t g1 = gcd(candidate, largest);
            seq_t g2 = gcd(candidate, second_max);
            /* If second_max is 1, the condition on g2 is automatically satisfied. */
            bool ok = (second_max == (seq_t)1 || g2 > (seq_t)1) &&
                      (largest == (seq_t)1   || g1 > (seq_t)1);
            if (ok) break;
            ++candidate;
        }

        /* Append the chosen candidate. */
        grow_seq_arr(seq_len + 1);
        seq_arr[seq_len] = candidate;

        /* Update largest and second largest for next iteration. */
        if (candidate > largest) {
            second_max = largest;
            largest = candidate;
        } else if (candidate > second_max) {
            second_max = candidate;
        }
    }

    return seq_arr[n-1];
}
