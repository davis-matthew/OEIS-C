/*
 * A249048  Self-numbers (A003052) that are multiples of 9.
 * https://oeis.org/A249048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 712, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: multiples of 9 that are self‑numbers.
   Computed by brute‑force search and caching. */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------
   Cached array of computed terms.
   -------------------------------------------------------------------- */
static seq_t *cache      = NULL;   /* array of found terms          */
static size_t cache_cap  = 0;      /* allocated capacity (in elements) */
static idx_t  cache_size = 0;      /* number of terms currently stored */

/* --------------------------------------------------------------------
   next_candidate holds the next multiple of 9 to test.
   -------------------------------------------------------------------- */
static seq_t next_candidate = 9;

/* --------------------------------------------------------------------
   Ensure that the cache has room for at least 'need' elements.
   -------------------------------------------------------------------- */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------
   Return the decimal digit sum of a number.
   -------------------------------------------------------------------- */
static seq_t digit_sum(seq_t n)
{
    seq_t s = 0;
    while (n != 0) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

/* --------------------------------------------------------------------
   Return the number of decimal digits of a number.
   -------------------------------------------------------------------- */
static idx_t digit_count(seq_t n)
{
    idx_t d = 0;
    if (n == 0)
        return 1;
    while (n != 0) {
        ++d;
        n /= 10;
    }
    return d;
}

/* --------------------------------------------------------------------
   Test whether x is a self‑number (no m such that m + digit_sum(m) = x).
   Only the relevant range of m is searched.
   -------------------------------------------------------------------- */
static bool is_self_number(seq_t x)
{
    idx_t d = digit_count(x);
    seq_t diff = (seq_t)(9 * d);               /* smallest possible generator offset */
    seq_t start = (x > diff) ? (x - diff) : 1; /* lower bound for possible generators */

    for (seq_t m = start; m < x; ++m) {
        if (m + digit_sum(m) == x)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------
   Main sequence function.  Computes a(n) for n >= 1.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1; undefined for n < 1  */

    /* Generate terms until we have n of them */
    while (cache_size < n) {
        if (is_self_number(next_candidate)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = next_candidate;
        }
        next_candidate += 9;   /* next multiple of 9 */
    }
    return cache[n - 1];
}
