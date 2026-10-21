/*
 * A283899  Relative of Hofstadter Q-sequence: a(-399) = 400, a(-398) = 4,
 *          a(-397) = 400, a(-396) = 4; thereafter a(n) = a(n-a(n-1)) +
 *          a(n-a(n-2)).
 * https://oeis.org/A283899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 465, u32 n <= 465, u64 n <= 465, u128 n <= 465,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Relative of Hofstadter Q-sequence (A283899)
 *
 *  Formula:  a(n) = a(n-a(n-1)) + a(n-a(n-2))
 *  Initial values:  a(-399) = 400, a(-398) = 4,
 *                   a(-397) = 400, a(-396) = 4,
 *  with convention a(n)=0 for n <= -400.
 *  Offset: 1 (a(1) is the first term in the b-file).
 *  The sequence terminates at n = 465 (a(465)=0); for n > 465 it is
 *  considered undefined.  This implementation returns 0 for n > 465.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#define OFFSET 400         /* mapping real index k to array[i] : i = k + OFFSET */

static seq_t *seq_cache = NULL;      /* memoisation array */
static size_t seq_cache_size = 0;    /* current allocation size */
static idx_t seq_max_computed = 0;   /* largest positive index that has been computed */
static bool seq_neg_done = false;    /* flag whether negative part has been initialised */

/* Ensure cache holds at least 'min_size' entries, zero-filling any new area */
static void seq_reserve(size_t min_size)
{
    if (seq_cache_size < min_size) {
        seq_t *new_cache = (seq_t*)realloc(seq_cache, min_size * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        /* Zero the newly added part */
        for (size_t i = seq_cache_size; i < min_size; ++i) new_cache[i] = 0;
        seq_cache = new_cache;
        seq_cache_size = min_size;
    }
}

/* Initialise negative indices up to 0 (inclusive) once */
static void seq_init_negatives(void)
{
    if (seq_neg_done) return;

    /* we must have space for indices -399 .. 0  (i.e. OFFSET+1 slots) */
    seq_reserve(OFFSET + 1);

    /* Base values */
    seq_cache[OFFSET - 399] = (seq_t)400;
    seq_cache[OFFSET - 398] = (seq_t)4;
    seq_cache[OFFSET - 397] = (seq_t)400;
    seq_cache[OFFSET - 396] = (seq_t)4;

    /* Compute -395 down to 0 using the recurrence */
    for (int i = -395; i <= 0; ++i) {
        size_t idx     = (size_t)(i + OFFSET);
        size_t idx_im1 = idx - 1;      /* i-1 */
        size_t idx_im2 = idx - 2;      /* i-2 */

        seq_t a_im1 = seq_cache[idx_im1];
        seq_t a_im2 = seq_cache[idx_im2];

        seq_t left, right;

        /* left part: a(i - a(i-1)) */
        if (a_im1 > (seq_t)(i + 400))    /* index would be <= -400 -> zero */
            left = 0;
        else {
            long long left_idx = (long long)i - (long long)a_im1;
            left = seq_cache[(size_t)(left_idx + OFFSET)];
        }

        /* right part: a(i - a(i-2)) */
        if (a_im2 > (seq_t)(i + 400))
            right = 0;
        else {
            long long right_idx = (long long)i - (long long)a_im2;
            right = seq_cache[(size_t)(right_idx + OFFSET)];
        }

        seq_cache[idx] = left + right;
    }

    seq_neg_done = true;
    seq_max_computed = 0;   /* we have computed up to index 0 now */
}

/* Public function that returns a(n) for n >= 1 (values for n > 465 are 0) */
seq_t a(idx_t n)
{
    if (n > 465) return 0;          /* sequence defined only up to 465 */
    if (n < 1)   return 0;          /* non‑positive indices are out of scope */

    seq_init_negatives();           /* guarantees negative part is ready */

    /* Ensure cache can hold up to n */
    size_t needed = (size_t)n + OFFSET + 1;
    seq_reserve(needed);

    /* Compute missing positive indices up to n */
    if (seq_max_computed < n) {
        for (idx_t i = seq_max_computed + 1; i <= n; ++i) {
            size_t idx     = (size_t)(i + OFFSET);
            size_t idx_im1 = idx - 1;
            size_t idx_im2 = idx - 2;

            seq_t a_im1 = seq_cache[idx_im1];
            seq_t a_im2 = seq_cache[idx_im2];

            seq_t left, right;

            /* left part */
            if (a_im1 > (seq_t)(i + 400))
                left = 0;
            else {
                long long left_idx = (long long)i - (long long)a_im1;
                left = seq_cache[(size_t)(left_idx + OFFSET)];
            }

            /* right part */
            if (a_im2 > (seq_t)(i + 400))
                right = 0;
            else {
                long long right_idx = (long long)i - (long long)a_im2;
                right = seq_cache[(size_t)(right_idx + OFFSET)];
            }

            seq_cache[idx] = left + right;
        }
        seq_max_computed = n;
    }

    return seq_cache[(size_t)(n + OFFSET)];
}
