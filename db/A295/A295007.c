/*
 * A295007  Numbers k such that the largest digit of k^2 is 7.
 * https://oeis.org/A295007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 29, u32 n <= 6378, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search: numbers k such that the largest digit of k^2 is 7 */
#include <stdlib.h>
#include <stdbool.h>

/* Static state for the cache */
static seq_t *cache = NULL;   /* allocated array of terms          */
static size_t capacity = 0;   /* size of the allocation            */
static size_t count    = 0;   /* number of terms stored            */
static seq_t next_k    = 1;   /* next k to test                    */

/* Return the largest decimal digit of x.  Works for any seq_t that
 * supports >, /, % operators. */
static unsigned largest_digit(seq_t x)
{
    unsigned max = 0;
    const seq_t ten = (seq_t)10;
    while (x > 0) {
        seq_t r = x % ten;
        unsigned d = (unsigned)r;      /* r < 10, safe to cast */
        if (d > max) max = d;
        x /= ten;
    }
    return max;
}

/* Ensure that the cache contains at least 'need' terms. */
static void ensure_cache(size_t need)
{
    /* Grow the allocation if necessary.  Double each time for amortised
     * linear cost. */
    if (capacity < count + (need - count)) {
        size_t new_cap = capacity ? capacity << 1 : 8;
        if (new_cap < count + (need - count))
            new_cap = count + (need - count);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();          /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* Generate more terms until we have at least 'need' of them. */
    while (count < need) {
        seq_t sq = next_k * next_k;
        if (largest_digit(sq) == 7)
            cache[count++] = next_k;
        next_k++;
    }
}

/* Retrieve the n-th term of the sequence (1‑based).  Undefined behaviour
 * for n < 1, but the function returns 0 in that case. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
