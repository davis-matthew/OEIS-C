/*
 * A334384  Lexicographically earliest sequence of distinct positive integers
 *          such that for any n > 0, a(3*n-2), a(3*n-1) and a(3*n) are in
 *          geometric progression.
 * https://oeis.org/A334384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 347, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334384: lexicographically earliest permutation of
   the natural numbers such that for every n ≥ 1, the triple
   a(3n-2), a(3n-1), a(3n) forms a geometric progression. */
/* This implementation builds the sequence on demand using memoisation
   and a simple search for the next admissible element in each
   position.  The algorithm follows the OEIS definition directly. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* Ensure the used‑array is large enough to hold index 'idx'.  */
/* The array stores a single byte per integer; 0 = unused, 1 = used. */
static void ensure_used_cap(unsigned char **used, size_t *cap, size_t idx)
{
    if (idx < *cap)
        return;
    size_t new_cap = (*cap == 0 ? 64 : *cap << 1);
    while (new_cap <= idx)
        new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(*used, new_cap);
    if (!tmp)
        exit(1);
    memset(tmp + *cap, 0, new_cap - *cap);
    *used = tmp;
    *cap  = new_cap;
}

/* Ensure the sequence array is large enough to hold index 'idx'. */
static void ensure_seq_cap(seq_t **seq, size_t *cap, size_t idx)
{
    if (idx < *cap)
        return;
    size_t new_cap = (*cap == 0 ? 64 : *cap << 1);
    while (new_cap <= idx)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*seq, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    *seq = tmp;
    *cap = new_cap;
}

/* Public function.  */
seq_t a(idx_t n)
{
    if (n == 0)       /* OEIS offset is 1 */
        return 0;

    /* Static state for memoisation.  */
    static seq_t *seq_arr   = NULL;
    static size_t seq_cap   = 0;
    static size_t seq_len   = 0;   /* largest index computed so far */

    static unsigned char *used      = NULL;
    static size_t used_cap           = 0;

    static uint64_t next_unused = 1;  /* smallest unused integer for a(3k-2) */

    /* Initialise containers on first call.  */
    if (!seq_arr) {
        seq_cap = 64;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_arr)
            exit(1);
        seq_arr[0] = 0;   /* sentinel – index 0 is unused */

        used_cap = 64;
        used = (unsigned char *)calloc(used_cap, 1);
        if (!used)
            exit(1);
    }

    size_t idx = (size_t)n;   /* safe cast – n ≥ 1, fits in size_t */

    /* Compute values up to 'idx' if not already done.  */
    if (idx > seq_len) {
        for (size_t i = seq_len + 1; i <= idx; ++i) {
            ensure_seq_cap(&seq_arr, &seq_cap, i);

            switch (i % 3) {

            case 1: {   /* a(3k-2) – first of a triple */
                size_t v = next_unused;
                /* Find smallest unused integer. */
                while (true) {
                    ensure_used_cap(&used, &used_cap, v);
                    if (!used[v])
                        break;
                    ++v;
                }
                seq_arr[i] = (seq_t)v;
                used[v] = 1;

                /* Advance next_unused to the next unused integer. */
                size_t w = v + 1;
                while (true) {
                    ensure_used_cap(&used, &used_cap, w);
                    if (!used[w])
                        break;
                    ++w;
                }
                next_unused = w;
                break;
            }

            case 2: {   /* a(3k-1) – second of a triple */
                seq_t x = seq_arr[i - 1];
                seq_t chosen_y = 0;
                for (uint64_t y = 1; ; ++y) {
                    if (y == (uint64_t)x)
                        continue;
                    size_t y_idx = (size_t)y;
                    ensure_used_cap(&used, &used_cap, y_idx);
                    if (used[y_idx])
                        continue;

                    seq_t y_sq = (seq_t)y * (seq_t)y;
                    if (y_sq % x != 0)
                        continue;

                    seq_t z = y_sq / x;
                    if (z == x || z == (seq_t)y)
                        continue;

                    size_t z_idx = (size_t)z;
                    ensure_used_cap(&used, &used_cap, z_idx);
                    if (used[z_idx])
                        continue;

                    chosen_y = (seq_t)y;
                    used[y_idx] = 1;
                    break;
                }
                seq_arr[i] = chosen_y;
                break;
            }

            case 0: {   /* a(3k) – third of a triple */
                seq_t x = seq_arr[i - 2];
                seq_t y = seq_arr[i - 1];
                seq_t y_sq = (seq_t)y * (seq_t)y;
                seq_t z = y_sq / x;
                size_t z_idx = (size_t)z;
                ensure_used_cap(&used, &used_cap, z_idx);
                seq_arr[i] = z;
                used[z_idx] = 1;
                break;
            }
            }
        }
        seq_len = idx;
    }

    return seq_arr[idx];
}
