/*
 * A327762  a(n) = smallest positive number not already in the sequence such
 *          that all n(n+1)/2 numbers in the triangle of differences of the
 *          first n terms are distinct.
 * https://oeis.org/A327762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   cmp_long
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = smallest positive integer not already in the sequence
 * such that the triangle of differences of the first n terms contains
 * n(n+1)/2 distinct values.  The sequence is finite (56 terms). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/cmp_long.h"

/* Check that all entries in the difference triangle of arr[0..len-1] are distinct. */
static bool triangle_distinct(const seq_t *arr, size_t len) {
    /* Max entries for n = 56: 56*57/2 = 1596 */
    static long long diffs[1596];
    size_t idx = 0;

    long long cur[60];
    long long nxt[59];

    for (size_t i = 0; i < len; ++i)
        cur[i] = (long long)arr[i];

    size_t cur_len = len;

    /* First row – the original values. */
    for (size_t i = 0; i < cur_len; ++i)
        diffs[idx++] = cur[i];

    /* Remaining rows. */
    while (cur_len > 1) {
        for (size_t i = 0; i < cur_len - 1; ++i)
            nxt[i] = cur[i + 1] - cur[i];
        cur_len--;
        for (size_t i = 0; i < cur_len; ++i)
            diffs[idx++] = nxt[i];
        for (size_t i = 0; i < cur_len; ++i)
            cur[i] = nxt[i];
    }

    /* Validate uniqueness. */
    qsort(diffs, idx, sizeof(long long), cmp_long);
    for (size_t i = 1; i < idx; ++i)
        if (diffs[i] == diffs[i - 1])
            return false;
    return true;
}

/* Cached sequence (max 56 terms). */
static seq_t seq[56];
static size_t seq_len = 0;

/* Incrementally build the sequence up to n (capped at 56). */
static void build_seq_up_to(idx_t n) {
    size_t target = (n < 57 ? (size_t)n : 56);
    while (seq_len < target) {
        seq_t cand = 1;
        for (;;) {
            /* Skip numbers already present. */
            bool already = false;
            for (size_t i = 0; i < seq_len; ++i) {
                if (seq[i] == cand) {
                    already = true;
                    break;
                }
            }
            if (already) {
                ++cand;
                continue;
            }

            /* Construct a temporary array with the candidate appended. */
            seq_t tmp[56];
            for (size_t i = 0; i < seq_len; ++i)
                tmp[i] = seq[i];
            tmp[seq_len] = cand;

            if (triangle_distinct(tmp, seq_len + 1))
                break; /* accept candidate */

            ++cand;
        }
        seq[seq_len++] = cand;
    }
}

/* Public interface. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    if (n > 56)          /* sequence terminates after 56 terms. */
        return 0;
    build_seq_up_to(n);
    return seq[(size_t)n - 1];
}
