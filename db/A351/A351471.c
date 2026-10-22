/*
 * A351471  Numbers m such that the largest digit in the decimal expansion of
 *          1/m is 5.
 * https://oeis.org/A351471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A351471: numbers m such that the largest decimal digit
   of 1/m is 5.  The function returns a(n) for 1‑based index n.
   It scans integers, checks the property by generating the decimal
   expansion of 1/m digit by digit, and caches the found terms for
   future calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Helper for tracking seen remainders during a single decimal expansion
   --------------------------------------------------------------- */
static seq_t *seen_rems = NULL;   /* array of stored remainders */
static size_t seen_cap = 0;       /* capacity of the array */
static size_t seen_len = 0;       /* number of elements stored */

/* Ensure the seen_rems array has capacity for at least 'need' elements  */
static void ensure_seen_cap(size_t need)
{
    if (seen_cap >= need) return;
    size_t new_cap = seen_cap ? seen_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(seen_rems, new_cap * sizeof(seq_t));
    seen_rems = tmp;
    seen_cap = new_cap;
}

/* Linear search: true if 'val' is already stored           */
static bool seen_contains(seq_t val)
{
    for (size_t i = 0; i < seen_len; ++i)
        if (seen_rems[i] == val) return true;
    return false;
}

/* Record a new remainder                        */
static void seen_add(seq_t val)
{
    ensure_seen_cap(seen_len + 1);
    seen_rems[seen_len++] = val;
}

/* Test whether the largest decimal digit in 1/m is exactly 5        */
static bool seq_is_good(seq_t m)
{
    if (m <= (seq_t)1) return false;

    seen_len = 0;          /* reset for this call */
    seq_t r = 1;           /* initial remainder */
    bool has5 = false;

    for (;;) {
#if defined(__SIZEOF_INT128__)
        unsigned __int128 prod = (unsigned __int128)r * 10;
        seq_t d = (seq_t)(prod / m);
        seq_t new_r = (seq_t)(prod % m);
#else
        /* Fallback when 128‑bit arithmetic is not available;
           it is only used for seq_t types that fit in 64 bits.  */
        seq_t prod = r * 10;
        seq_t d = prod / m;
        seq_t new_r = prod % m;
#endif
        if (d > (seq_t)5) return false;
        if (!has5 && d == (seq_t)5) has5 = true;
        if (new_r == (seq_t)0) return has5;
        if (seen_contains(new_r)) return has5;  /* cycle detected */

        seen_add(new_r);
        r = new_r;
    }
}

/* ------------------------------------------------------------------ */
/* Cache for generated sequence terms                                 *
   --------------------------------------------------------------- */
static seq_t *cache      = NULL;   /* dynamic array of terms found so far   */
static size_t  cache_cap = 0;      /* capacity of cache                      */
static size_t  cache_len = 0;      /* number of desired terms stored         */
static idx_t   next_candidate = 2; /* next integer to test                  */

/* Ensure cache can hold at least 'needed' elements                    */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Public function required by the task                               *
   --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;  /* offset is 1 */

    /* Generate terms until we have at least n terms. */
    while ((idx_t)cache_len < n) {
        if (seq_is_good((seq_t)next_candidate)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
