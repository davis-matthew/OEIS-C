/*
 * A251655  4-step Fibonacci sequence starting with 0, 1, 1, 1.
 * https://oeis.org/A251655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = a(n)+a(n+1)+a(n+2)+a(n+3), a(0)=0, a(1)=1, a(2)=1,
 *            a(3)=1
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached values of the sequence.  The cache is grown on demand.
 * Static storage ensures that successive calls reuse previously
 * computed terms even when n is not monotonic. */
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;   /* allocated size of seq_cache */
static idx_t seq_max = 0;    /* largest index currently stored */

/* Return the n‑th term of the 4‑step Fibonacci sequence. */
seq_t a(idx_t n)
{
    /* Initialise the cache with the first four terms on first call. */
    if (seq_cache == NULL) {
        size_t init_cap = 4;
        seq_cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!seq_cache) return (seq_t)0;   /* out of memory */
        seq_cache[0] = (seq_t)0;
        seq_cache[1] = (seq_t)1;
        seq_cache[2] = (seq_t)1;
        seq_cache[3] = (seq_t)1;
        seq_cap = init_cap;
        seq_max = 3;
    }

    /* If the requested term is already cached, return it. */
    if (n <= seq_max) {
        return seq_cache[(size_t)n];
    }

    /* Ensure the cache is large enough to hold up to index n. */
    if ((size_t)(n + 1) > seq_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* out of memory */
        seq_cache = tmp;
        seq_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = seq_max + 1; i <= n; ++i) {
        seq_cache[(size_t)i] =
            seq_cache[(size_t)(i - 1)] +
            seq_cache[(size_t)(i - 2)] +
            seq_cache[(size_t)(i - 3)] +
            seq_cache[(size_t)(i - 4)];
    }
    seq_max = n;
    return seq_cache[(size_t)n];
}
