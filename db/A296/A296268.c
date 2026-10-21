/*
 * A296268  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)*b(n), where a(0) = 1, a(1) = 4, b(0) = 2, b(1) = 3, b(2) =
 *          5, and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4; for n≥2, a(n)=a(n‑1)+a(n‑2)+b(n‑2)*b(n) * where
 *            b(0)=2, b(1)=3, b(2)=5 and the sequences (a) and (b) are
 *            increasing complementary
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for the computed sequences */
static seq_t *a_arr = NULL;
static seq_t *b_arr = NULL;
static idx_t a_used = 0, a_cap = 0;
static idx_t b_used = 0, b_cap = 0;
static bool init_done = false;

/* Ensure that an array has at least 'need' slots. */
static void ensure_cap(idx_t *cap, seq_t **arr, idx_t need)
{
    if (*cap >= need) return;
    idx_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Return true if 'value' appears in a_arr[0 .. size-1]. */
static bool is_in_a(idx_t size, seq_t value)
{
    size_t lo = 0, hi = (size_t)size;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t midv = a_arr[mid];
        if (midv == value) return true;
        if (midv < value)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Lazy initialization of base prefixes. */
    if (!init_done) {
        init_done = true;
        ensure_cap(&a_cap, &a_arr, 2);   /* a(0) and a(1) */
        ensure_cap(&b_cap, &b_arr, 3);   /* b(0), b(1), b(2) */
        a_arr[0] = (seq_t)1;
        a_arr[1] = (seq_t)4;
        b_arr[0] = (seq_t)2;
        b_arr[1] = (seq_t)3;
        b_arr[2] = (seq_t)5;
        a_used = 2;
        b_used = 3;
    }

    /* Return instantly if we already know the term. */
    if (n < a_used) return a_arr[(size_t)n];

    /* Grow the sequences up to index n. */
    while (a_used <= n) {
        idx_t i = a_used;                       /* current index to compute */

        /* Ensure b(i) exists.  For i ≥ 3, compute the next complementary value. */
        if (i >= b_used) {
            seq_t cand = b_arr[b_used - 1] + (seq_t)1;
            /* Skip numbers already in the a-sequence (complement property). */
            while (is_in_a(i, cand)) cand += (seq_t)1;
            ensure_cap(&b_cap, &b_arr, b_used + 1);
            b_arr[b_used++] = cand;
        }

        /* Compute a(i) using the recurrence. */
        seq_t prod = b_arr[i - 2] * b_arr[i];
        seq_t val  = a_arr[i - 1] + a_arr[i - 2] + prod;
        ensure_cap(&a_cap, &a_arr, a_used + 1);
        a_arr[a_used++] = val;
    }

    return a_arr[(size_t)n];
}
