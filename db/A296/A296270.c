/*
 * A296270  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)*b(n), where a(0) = 2, a(1) = 4, b(0) = 1, b(1) = 3, b(2) =
 *          5, and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2)*b(n) a(0) = 2, a(1) = 4; b(0) =
 *            1, b(1) = 3, b(2) = 5;
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Ensure the dynamic array pointed to by *ptr has capacity at least "needed". */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap < needed) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        while (newcap < needed)
            newcap <<= 1;
        *ptr = realloc(*ptr, newcap * elem_size);
        *cap = newcap;
    }
}

/* Return a(n). */
seq_t a(idx_t n)
{
    /* Static state: the two sequences and their lengths/capacities. */
    static seq_t *a_seq = NULL;
    static seq_t *b_seq = NULL;
    static size_t a_len = 0;
    static size_t b_len = 0;
    static size_t a_cap = 0;
    static size_t b_cap = 0;
    static size_t a_scan = 0; /* index into a_seq for membership test */
    static size_t b_scan = 0; /* index into b_seq for membership test */

    /* One‑time initialization: set up the initial values of a and b. */
    if (a_len == 0) {
        ensure_capacity(2, &a_cap, (void **)&a_seq, sizeof(seq_t));
        ensure_capacity(3, &b_cap, (void **)&b_seq, sizeof(seq_t));

        a_seq[0] = (seq_t)2;
        a_seq[1] = (seq_t)4;
        a_len = 2;

        b_seq[0] = (seq_t)1;
        b_seq[1] = (seq_t)3;
        b_seq[2] = (seq_t)5;
        b_len = 3;
    }

    /* If the requested index is already computed, return it immediately. */
    size_t idx = (size_t)n;
    if (idx < a_len)
        return a_seq[idx];

    /* Compute terms up to index n. */
    for (size_t i = a_len; i <= idx; ++i) {
        seq_t bi;
        if (i < b_len) {
            bi = b_seq[i];
        } else {
            /* Find the next unused integer for b(i). */
            seq_t cand = b_seq[b_len - 1] + 1;
            for (;;) {
                while (a_scan < a_len && a_seq[a_scan] < cand)
                    ++a_scan;
                while (b_scan < b_len && b_seq[b_scan] < cand)
                    ++b_scan;
                bool used = (a_scan < a_len && a_seq[a_scan] == cand) ||
                            (b_scan < b_len && b_seq[b_scan] == cand);
                if (used) {
                    ++cand;
                    continue;
                }
                break;
            }
            bi = cand;
            ensure_capacity(b_len + 1, &b_cap, (void **)&b_seq, sizeof(seq_t));
            b_seq[b_len++] = bi;
        }

        /* Compute a(i) using the recurrence. */
        seq_t ai = a_seq[i - 1] + a_seq[i - 2] + b_seq[i - 2] * bi;

        ensure_capacity(a_len + 1, &a_cap, (void **)&a_seq, sizeof(seq_t));
        a_seq[a_len++] = ai;
    }

    return a_seq[idx];
}
