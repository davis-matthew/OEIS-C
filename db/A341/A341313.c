/*
 * A341313  a(n) = (a(n-1) + a(n-3))/2^m, where 2^m is the highest power of 2
 *          that divides both a(n-1) and a(n-3), with a(0) = a(1) = a(2) = 1.
 * https://oeis.org/A341313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (a(n-1) + a(n-3)) / 2^m, where m is the largest integer
 *            such that * 2^m divides both a(n-1) and a(n-3); a(0) = a(1) =
 *            a(2) = 1
 * Range:     u8 n <= 33, u16 n <= 50, u32 n <= 85, u64 n <= 174,
 *            u128 n <= 323, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation memoises previously computed terms in a static array so that
 * each value is calculated only once.  Subsequent calls to a(n) return the cached
 * value immediately.  All operations are performed using the integer types
 * seq_t and idx_t supplied by the build system.
 */

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of the required integer types.  They are defined
 * by the build system: idx_t is an unsigned 64‑bit integer,
 * seq_t is an unsigned integer of 8/16/32/64 bits or an arbitrary‑precision
 * integer type.  No definitions are provided here. */

static seq_t *cache = NULL;          /* memoisation table (0 means “not yet computed”) */
static idx_t cache_cap = 0;          /* current capacity of the table */
static idx_t cache_next = 0;         /* index of the first element that is not yet computed */

/* Ensure the cache array can hold indices up to 'needed' (inclusive).  The
 * array is always guaranteed to be large enough for at least three elements
 * so that a(0), a(1) and a(2) can be stored. */
static void ensure_capacity(idx_t needed)
{
    if (needed < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap <= needed)
        new_cap = needed + 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) {
        /* In a production environment you would handle out‑of‑memory
         * conditions appropriately.  For this challenge we simply exit. */
        exit(EXIT_FAILURE);
    }
    /* Initialise any newly allocated entries to 0 (meaning “not yet
     * computed”).  The classic C behaviour of realloc() does not
     * zero the new area. */
    if (new_cap > cache_cap)
        for (idx_t i = cache_cap; i < new_cap; ++i)
            new_arr[i] = 0;
    cache = new_arr;
    cache_cap = new_cap;
}

/* Count trailing zero bits of a positive seq_t value.  The result is
 * stored in an unsigned 64 bit integer because the exponent m can
 * never exceed that width. */
static uint64_t trailing_zeros(seq_t x)
{
    uint64_t cnt = 0;
    while ((x & (seq_t)1) == (seq_t)0) {
        x >>= 1;
        ++cnt;
    }
    return cnt;
}

/* Compute a(n), using memoisation and iterative filling of the cache. */
seq_t a(idx_t n)
{
    /* Initialise the memoisation table on the first call. */
    if (!cache) {
        ensure_capacity(3);
        /* a(0), a(1), a(2) are all 1. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_next = 3;
    }

    /* Make sure the cache can hold the requested index. */
    ensure_capacity(n);

    /* Fill the cache up to n if it is still incomplete. */
    for (idx_t i = cache_next; i <= n; ++i) {
        seq_t val;
        if (i < 3) {
            /* These cases are already initialised, but we set them
             * again for clarity. */
            val = (seq_t)1;
        } else {
            seq_t a_prev   = cache[i - 1];
            seq_t a_prev3  = cache[i - 3];
            uint64_t v1 = trailing_zeros(a_prev);
            uint64_t v3 = trailing_zeros(a_prev3);
            uint64_t m  = v1 < v3 ? v1 : v3;
            val = (a_prev + a_prev3) >> m;
        }
        cache[i] = val;
    }
    cache_next = n + 1;

    return cache[n];
}
