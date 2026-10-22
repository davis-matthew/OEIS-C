/*
 * A391025  Odd numbers that are neither powerful nor squarefree.
 * https://oeis.org/A391025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd numbers that are neither powerful nor squarefree
 *            (computed by sequential search)
 * Range:     u8 n <= 13, u16 n <= 6012, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_mixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_mixed.h"

/* Cached sequence values */
static seq_t *seq_vals = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t next_candidate = 1;    /* will become 3 on first use */

/* Ensure the sequence cache has space for at least n terms */
static void ensure_seq_cap(size_t n)
{
    if (n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    if (new_cap < n) new_cap = n;
    /* cast is needed for C++ compatibility */
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_vals = tmp;
    seq_cap = new_cap;
}

/* Generate sequence terms until at least n terms are known */
static void generate_until(idx_t n)
{
    while (seq_len < (size_t)n) {
        next_candidate += 2;                /* jump to next odd number */
        if (is_mixed(next_candidate)) {
            ensure_seq_cap(seq_len + 1);
            seq_vals[seq_len++] = (seq_t)next_candidate;
        }
    }
}

/* External sequence function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; defensive zero */
    generate_until(n);
    return seq_vals[n - 1];
}
