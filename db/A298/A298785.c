/*
 * A298785  Partial sums of A298784.
 * https://oeis.org/A298785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) a(0) = 1,
 *            a(1) = 5, a(2) = 11, a(3) = 21, a(4) = 35
 * Range:     u8 n <= 11, u16 n <= 197, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* array that holds computed terms     */
static size_t cap   = 0;      /* total capacity (number of elements) */
static size_t len   = 0;      /* highest index that has been computed */

/* Ensure that the cache can hold indices 0 … n (inclusive). */
static void ensure_cap(size_t n)
{
    if (cap <= n) {
        size_t new_cap = (cap > 0 ? cap * 2 : 8);
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }
}

/* Return the n‑th term of sequence A298785. */
seq_t a(idx_t n)
{
    /* Convert the unsigned 64‑bit index to size_t for array access. */
    size_t idx = (size_t)n;

    /* Initialise base values on first call. */
    if (len == 0) {
        ensure_cap(4);                /* indices 0 … 4 needed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)21;
        cache[4] = (seq_t)35;
        len = 5;                      /* values 0–4 are ready */
    }

    /* If the caller requests an index already known, just return it. */
    if (idx < len) {
        return cache[idx];
    }

    /* Make sure we have room for the requested indices. */
    if (idx + 1 > cap) {
        ensure_cap(idx);
    }

    /* Compute any missing terms up to idx. */
    for (size_t i = len; i <= idx; ++i) {
        /* recurrence: 2*a(i-1) - a(i-2) + a(i-3) - 2*a(i-4) + a(i-5) */
        seq_t val = cache[i-1];
        val += cache[i-1];          /* 2*a(i-1) */
        val -= cache[i-2];          /* -a(i-2) */
        val += cache[i-3];          /* +a(i-3) */
        val -= cache[i-4];
        val -= cache[i-4];          /* -2*a(i-4) */
        val += cache[i-5];          /* +a(i-5) */
        cache[i] = val;
    }

    len = idx + 1;                  /* all indices up to idx are now known */
    return cache[idx];
}
