/*
 * A394576  a(1) = 1; a(n+1) is the smallest positive integer greater than
 *          a(n) that avoids violating the condition a(k) + a(k+1) + a(k+2) is
 *          not a term.
 * https://oeis.org/A394576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n) is the smallest integer > a(n-1) such that
 *            * a(k)+a(k+1)+a(k+2) is not a term of the sequence for all k. *
 *            The function memoizes previously computed values and works in
 *            any call order
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Dynamic array for the sequence */
static seq_t *seq_buf = NULL;
static size_t seq_cap = 0;
static idx_t seq_len = 0;

/* Dynamic array for the sums of consecutive triples */
static seq_t *sum_buf = NULL;
static size_t sum_cap = 0;
static idx_t sum_len = 0;

/* Ensure seq_buf can hold at least new_cap elements. */
static void ensure_seq_capacity(size_t new_cap)
{
    if (new_cap <= seq_cap) return;
    size_t cap = seq_cap > 0 ? seq_cap : 8;
    while (cap < new_cap) cap <<= 1;
    seq_buf = (seq_t *)realloc(seq_buf, cap * sizeof(seq_t));
    seq_cap = cap;
}

/* Ensure sum_buf can hold at least new_cap elements. */
static void ensure_sum_capacity(size_t new_cap)
{
    if (new_cap <= sum_cap) return;
    size_t cap = sum_cap > 0 ? sum_cap : 8;
    while (cap < new_cap) cap <<= 1;
    sum_buf = (seq_t *)realloc(sum_buf, cap * sizeof(seq_t));
    sum_cap = cap;
}

/* Binary search: return 1 if val is in arr[0..len-1], else 0. */
static int contains(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] == val) return 1;
        if (arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but safe guard */

    /* Generate terms up to n if necessary. */
    while (seq_len < n) {
        /* Initialise the very first term. */
        if (seq_len == 0) {
            seq_t val = 1;
            ensure_seq_capacity(1);
            seq_buf[0] = val;
            seq_len = 1;
            continue;
        }

        /* Candidate is the next integer after the latest term. */
        seq_t cand = seq_buf[seq_len - 1] + 1;

        /* Avoid making the candidate equal to any existing triple sum. */
        while (contains(sum_buf, sum_len, cand))
            cand++;

        /* Avoid a new triple sum that would coincide with an existing term. */
        if (seq_len >= 2) {
            seq_t sum_last = seq_buf[seq_len - 2] + seq_buf[seq_len - 1] + cand;
            while (contains(seq_buf, seq_len, sum_last)) {
                cand++;
                sum_last = seq_buf[seq_len - 2] + seq_buf[seq_len - 1] + cand;
            }
        }

        /* Store the new term. */
        ensure_seq_capacity(seq_len + 1);
        seq_buf[seq_len] = cand;
        seq_len++;

        /* Record the sum of the new consecutive triple, if any. */
        if (seq_len >= 3) {
            seq_t new_sum = seq_buf[seq_len - 3] + seq_buf[seq_len - 2] + seq_buf[seq_len - 1];
            ensure_sum_capacity(sum_len + 1);
            sum_buf[sum_len] = new_sum;
            sum_len++;
        }
    }

    return seq_buf[n - 1];
}
