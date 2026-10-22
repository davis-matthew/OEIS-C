/*
 * A387683  a(0) = 0; thereafter a(n) = a(n-1) - n if a(n-1) >= 2*n and not
 *          already in the sequence, otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A387683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n>0, a(n) = a(n-1) - n if a(n-1) >= 2*n and *
 *            a(n-1)-n has not appeared before; otherwise a(n) = a(n-1) + n
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static state for memoisation */
static seq_t *seq     = NULL;   /* dynamic array of terms */
static size_t seq_cap = 0;      /* current capacity */
static size_t seq_len = 0;      /* number of stored terms */

/* Ensure the array has room for at least `need` elements. */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    /* In production code you might check !seq and handle OOM,
       but the test harness will terminate on malloc failure. */
    seq_cap = new_cap;
}

/* External function required by the specification */
seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (seq == NULL) {
        ensure_capacity(1);
        seq[0] = 0;
        seq_len = 1;
    }

    /* If the requested term is already computed, return it. */
    if ((size_t)n < seq_len)
        return seq[(size_t)n];

    /* Compute terms up to n incrementally. */
    for (idx_t i = (idx_t)seq_len; i <= n; ++i) {
        seq_t prev = seq[(size_t)i - 1];
        seq_t next;

        /* Check if we can subtract i. */
        if (prev >= 2 * (seq_t)i) {
            seq_t cand = prev - (seq_t)i;
            bool seen = false;
            for (size_t j = 0; j < seq_len - 1; ++j) {
                if (seq[j] == cand) {
                    seen = true;
                    break;
                }
            }
            if (!seen)
                next = cand;
            else
                next = prev + (seq_t)i;
        } else {
            next = prev + (seq_t)i;
        }

        ensure_capacity(seq_len + 1);
        seq[seq_len++] = next;
    }

    return seq[(size_t)n];
}
