/*
 * A319293  Numbers of the form 27^i*(9*j +- 1).
 * https://oeis.org/A319293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 9231, u32 n <= 9231, u64 n <= 9231,
 *            u128 n <= 9231, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers of the form 27^i*(9*j ± 1) in increasing order.
   The function fills a static cache, extending it when a larger index
   is requested.  It scans natural numbers, checking that the 3-adic
   valuation of x is a multiple of 3 and that x / 3^v3(x) ≡ ±1 (mod 9). */

#include <stdint.h>
#include <stdlib.h>

/* static cache variables */
static seq_t *seq_cache = NULL;   /* pointer to cached terms */
static size_t  seq_len   = 0;      /* number of terms cached so far */
static size_t  seq_cap   = 0;      /* capacity of the cache */
static seq_t  next_val   = 1;      /* next integer to test (scan position) */

/* helper: grow cache capacity if needed */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* allocation failed – abort (as per usual static libraries) */
        abort();
    }
    seq_cache = new_ptr;
    seq_cap   = new_cap;
}

/* helper: test whether x belongs to the sequence set */
static int is_candidate(seq_t x)
{
    seq_t t = x;
    idx_t exp = 0;
    while (t % 3 == 0) {
        t /= 3;
        ++exp;
    }
    if (exp % 3 != 0)
        return 0;
    seq_t r = t % 9;
    return (r == 1 || r == 8) ? 1 : 0;
}

/* external function implementing a(n) */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1, but guard */

    /* extend cache until we have at least n terms */
    while (seq_len < (size_t)n) {
        if (is_candidate(next_val)) {
            /* store the candidate */
            ensure_cache_cap(seq_len + 1);
            seq_cache[seq_len++] = next_val;
        }
        ++next_val;
    }
    /* return the n-th term (1-based index) */
    return seq_cache[n - 1];
}
