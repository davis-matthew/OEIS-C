/*
 * A331276  The numbers formed by removing the required digits to form the
 *          number from a set of digits which is initially empty and that has
 *          digits added via the addition of numbers which cannot be created
 *          from the digits currently in the set. Start by trying to create
 *          the number 0.
 * https://oeis.org/A331276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the bag‑of‑digits rule described in OEIS entry A331276.
   For each integer starting at 0, if the current bag of available
   digits contains all the digits needed to write that integer,
   the integer is created (its digits are removed from the bag) and
   appended to the sequence; otherwise all its digits are added to
   the bag.  The function `a(n)` returns the n‑th number (n≥1)
   produced by this process.  The implementation keeps a static cache
   of generated terms and expands it lazily. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* cached sequence values          */
static size_t cache_cap = 0;         /* capacity of cache               */
static size_t cache_len = 0;         /* number of values already cached */
static idx_t next_candidate = 0;     /* next integer to process         */
static int avail[10] = {0};          /* current bag (digit counts)      */

/* Ensure the cache has space for at least one more element. */
static void ensure_cache(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_arr;
        cache_cap = new_cap;
    }
}

/* Process the current integer next_candidate according to the rule. */
static void process_next(void)
{
    idx_t num = next_candidate;
    int need[10] = {0};

    /* Determine the multiset of digits required by num. */
    if (num == 0) {
        need[0] = 1;
    } else {
        idx_t t = num;
        while (t > 0) {
            int d = t % 10;
            need[d]++;
            t /= 10;
        }
    }

    /* Check whether the bag contains the required digits. */
    bool can_create = true;
    for (int d = 0; d < 10; ++d) {
        if (need[d] > avail[d]) {
            can_create = false;
            break;
        }
    }

    if (can_create) {
        /* Remove the required digits. */
        for (int d = 0; d < 10; ++d) avail[d] -= need[d];

        /* Append the created number to the cache. */
        ensure_cache();
        cache[cache_len++] = (seq_t)num;
    } else {
        /* Add all of num's digits to the bag. */
        for (int d = 0; d < 10; ++d) avail[d] += need[d];
    }

    /* Move to the next integer. */
    ++next_candidate;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for n<1, return 0 */

    /* Generate terms until we have at least n values cached. */
    while (cache_len < (size_t)n) {
        process_next();
    }

    return cache[n - 1];
}
