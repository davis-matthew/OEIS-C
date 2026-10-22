/*
 * A364448  Lexicographically earliest sequence where n is banned for n^2
 *          terms after each appearance.
 * https://oeis.org/A364448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A364448: lexicographically earliest sequence where each integer n is banned for the next n^2 positions after each appearance. The sequence is built greedily: at each step choose the smallest integer that is currently not banned. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Global storage for the sequence and the next available positions. */
static seq_t *seq_values = NULL;                 /* a(1), a(2), … */
static size_t seq_capacity = 0;                  /* allocated capacity */
static size_t seq_len = 0;                       /* number of terms stored */

static idx_t *next_avail = NULL;                 /* next index at which number k can appear again */
static size_t next_capacity = 0;                 /* allocated size */

/* Ensure seq_values can hold at least 'needed' elements. */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_capacity >= needed)
        return;
    size_t new_cap = seq_capacity ? seq_capacity : 1024;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_values = (seq_t *)realloc(seq_values, new_cap * sizeof(*seq_values));
    if (!seq_values)
        abort();
    seq_capacity = new_cap;
}

/* Ensure next_avail array can address index k (k ≥ 1). New entries are initialized to 1. */
static void ensure_next_capacity(idx_t k)
{
    if (next_capacity > (size_t)k)
        return;
    size_t new_cap = next_capacity ? next_capacity : 16;
    while (new_cap <= (size_t)k)
        new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(next_avail, new_cap * sizeof(*tmp));
    if (!tmp)
        abort();
    for (size_t i = next_capacity; i < new_cap; ++i)
        tmp[i] = 1;      /* first available position is 1 */
    next_avail = tmp;
    next_capacity = new_cap;
}

/* Return a(n) for 1‑based index n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, return 0 */

    /* Generate sequence lazily up to position n. */
    while (seq_len < (size_t)n) {
        idx_t current_index = (idx_t)(seq_len + 1);   /* 1‑based */

        /* Find smallest integer k whose next available index <= current index. */
        idx_t k = 1;
        for (;;) {
            ensure_next_capacity(k);
            if (next_avail[k] <= current_index)
                break;
            ++k;
        }

        /* Store chosen integer. */
        ensure_seq_capacity(seq_len + 1);
        seq_values[seq_len] = (seq_t)k;
        ++seq_len;

        /* Update next available index for k. */
        next_avail[k] = current_index + k * k + 1ULL;
    }

    return seq_values[n - 1];
}
