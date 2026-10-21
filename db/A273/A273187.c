/*
 * A273187  a(n) is the third number in a triple consisting of 3 numbers,
 *          which when squared are part of a right diagonal of magic square of
 *          squares.
 * https://oeis.org/A273187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) - 96; # for n >= 1 a(0) = 99, a(1) =
 *            449 This recurrence follows from a(n+1)=a(n)*6 - a(n-1) - 96
 *            with k=96
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache = NULL;   /* cached values */
static size_t cache_size = 0; /* number of cached entries */

/* Ensure that a(0) .. a(n) are computed and cached. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size) return;                   /* already computed */

    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {                                  /* allocation failure */
        /* In a real system we would handle the error; here we abort. */
        abort();
    }

    /* Initialize the new terms. */
    for (size_t i = cache_size; i < new_size; ++i) {
        if (i == 0) {
            new_cache[0] = (seq_t)99;
        } else if (i == 1) {
            new_cache[1] = (seq_t)449;
        } else if (i == 2) {
            new_cache[2] = (seq_t)2499;  /* 6*449 - 99 - 96 */
        } else {
            /* a[i] = 6*a[i-1] - a[i-2] - 96 */
            new_cache[i] = ((seq_t)6 * new_cache[i-1]) - new_cache[i-2] - (seq_t)96;
        }
    }

    cache = new_cache;
    cache_size = new_size;
}

/* Public function returning the n-th term of sequence A273187. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
