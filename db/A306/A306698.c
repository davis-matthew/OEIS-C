/*
 * A306698  Recamán-like sequence: a(0) = 0: for n > 0, a(n) = a(n-1) - 2*n if
 *          positive and not already in the sequence, otherwise a(n) = a(n-1)
 *          + n.
 * https://oeis.org/A306698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recamán-like sequence (OEIS A306698):
   a(0) = 0
   a(n) = a(n-1) - 2*n       if a(n-1) > 2*n   and this value has not appeared yet
           a(n-1) + n       otherwise
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static seq_t   *cache     = NULL;   /* sequence values */
static size_t  cache_len  = 0;      /* number of values stored (next free index) */
static size_t  cache_cap  = 0;      /* allocated capacity of cache */

static unsigned char *seen     = NULL; /* 0/1 flag array for values that have appeared */
static size_t        seen_cap = 0;     /* capacity of seen array */

/* Ensure that 'seen' array can hold values up to 1000000 at least; extend if needed */
static void ensure_seen_cap(size_t val)
{
    if (val < seen_cap) return;           /* already covered */
    size_t new_cap = seen_cap ? seen_cap << 1 : 64;
    while (new_cap <= val) new_cap <<= 1;
    unsigned char *new_seen = (unsigned char *)realloc(seen, new_cap);
    if (!new_seen) abort();               /* allocation failure */
    /* new entries are zero-initialized by realloc only if size increased: */
    if (new_cap > seen_cap) memset(new_seen + seen_cap, 0, new_cap - seen_cap);
    seen     = new_seen;
    seen_cap = new_cap;
}

/* Mark the value 'val' as seen, extending the array if necessary */
static void mark_seen(seq_t val)
{
    size_t v = (size_t)val;
    ensure_seen_cap(v);
    seen[v] = 1;
}

/* Check whether 'val' has appeared already; returns true if seen */
static bool is_seen(seq_t val)
{
    size_t v = (size_t)val;
    if (v >= seen_cap) return false;
    return seen[v];
}

/* Grow the cache array to at least 'min_cap' capacity */
static void ensure_cache_cap(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache     = new_cache;
    cache_cap = new_cap;
}

/* Compute the sequence up to index n (inclusive), storing results in static cache */
static void ensure_cache_len(idx_t n)
{
    /* initialize first term if needed */
    if (cache_len == 0) {
        ensure_cache_cap(1);
        cache[0] = (seq_t)0;
        mark_seen((seq_t)0);
        cache_len = 1;
    }
    /* generate terms until we have a(n) */
    while (cache_len <= (size_t)n) {
        idx_t cur_index = (idx_t)cache_len;          /* index of term to compute (0‑based) */
        seq_t prev = cache[cache_len - 1];
        seq_t next;

        /* Compute candidate = prev - 2*cur_index if prev > 2*cur_index */
        seq_t double_i = (seq_t)2 * (seq_t)cur_index;
        if (prev > double_i) {
            seq_t cand = prev - double_i;
            if (!is_seen(cand))
                next = cand;
            else
                next = prev + (seq_t)cur_index;
        } else {
            next = prev + (seq_t)cur_index;
        }

        ensure_cache_cap(cache_len + 1);
        cache[cache_len] = next;
        mark_seen(next);
        cache_len++;
    }
}

/* Public function as required by the build */
seq_t a(idx_t n)
{
    ensure_cache_len(n);
    return cache[n];
}
