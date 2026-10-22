/*
 * A337287  Numbers k such that both k and k+1 are in A095096.
 * https://oeis.org/A337287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A337287.
   Formula: numbers k such that both k and k+1 have even Zeckendorf digit-sum parity.
   The implementation performs a sequential search using Zeckendorf representation
   parity, without any pre‑computed table.  */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- Helper: Zeckendorf parity ---------------- */

static idx_t *fib = NULL;      /* Fibonacci array 1,2,3,5,... */
static size_t fib_len = 0;     /* number of entries in fib */
static size_t fib_cap = 0;     /* allocated capacity of fib */

/* Ensure fib contains all Fibonacci numbers up to at least 'max'.
   The array always contains a value > max. */
static void extend_fibs(idx_t max)
{
    if (fib_len == 0) {
        fib_cap = 16;
        fib = (idx_t *)realloc(fib, fib_cap * sizeof(idx_t));
        fib[0] = 1;
        fib[1] = 2;
        fib_len = 2;
    }
    while (fib_len >= 2 && fib[fib_len - 1] <= max) {
        if (fib_len >= fib_cap) {
            fib_cap <<= 1;
            fib = (idx_t *)realloc(fib, fib_cap * sizeof(idx_t));
        }
        idx_t next = fib[fib_len - 1] + fib[fib_len - 2];
        fib[fib_len++] = next;
        if (next < fib[fib_len - 2])  /* overflow guard */
            break;
    }
}

/* Largest Fibonacci <= x.  */
static size_t largest_fib_idx(idx_t x)
{
    size_t i = fib_len - 1;
    while (fib[i] > x)
        --i;
    return i;
}

/* Return true iff the Zeckendorf digit sum of x is even. */
static bool even_zeckendorf(idx_t x)
{
    if (x == 0)
        return true;          /* empty sum has even parity */

    extend_fibs(x);
    bool parity = false;      /* false means even count of digits seen so far */
    while (x > 0) {
        size_t i = largest_fib_idx(x);
        parity = !parity;     /* toggle for the digit we just used */
        x -= fib[i];
    }
    return !parity;           /* even parity iff parity==false */
}

/* ---------- Main sequence generator ---------------- */

static seq_t *cache = NULL;       /* cache of found sequence values */
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;  /* next integer to test */

/* Return the n-th term of A337287 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined, but safe */

    /* Compute terms until we have at least n entries. */
    while (cache_len < (size_t)n) {
        /* Ensure we can compute parity for the current candidate and the next. */
        idx_t k = next_candidate;
        if (even_zeckendorf(k) && even_zeckendorf(k + 1)) {
            /* Store k in the cache. */
            if (cache_len >= cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)k;
        }
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
