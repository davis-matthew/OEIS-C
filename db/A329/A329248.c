/*
 * A329248  Lexicographically earliest sequence S of distinct terms such that
 *          a(n) is divisible by the last odd digit of S, with a(1) = 1.
 * https://oeis.org/A329248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 174, u16 n <= 9571, u32 n <= 9571, u64 n <= 9571,
 *            u128 n <= 9571, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A329248.
   Lexicographically earliest permutation of the positive integers
   such that a(1)=1 and for n>1, a(n) is the smallest unused integer
   divisible by the last odd decimal digit seen in the sequence so far. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"

static seq_t *cache      = NULL;   /* memoised terms */
static size_t cache_cap  = 0;      /* capacity of cache array       */
static idx_t  cached     = 0;      /* number of terms already computed */

static uint8_t last_odd = 1;       /* most recent odd decimal digit */

static uint8_t *used      = NULL;  /* bitmap of used integers      */
static size_t   used_cap  = 0;     /* capacity of used bitmap      */

/* Ensure the cache can hold 'need' terms. */
static void ensure_cache(idx_t need)
{
    if ((size_t)need <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < (size_t)need) new_cap <<= 1;

    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Ensure the used bitmap can index up to 'need'. */
static void ensure_used(idx_t need)
{
    if ((size_t)need < used_cap) return;  /* indices 0..used_cap-1 are valid */

    size_t new_cap = used_cap ? used_cap << 1 : 1;
    while (new_cap <= (size_t)need) new_cap <<= 1;   /* inclusive of 'need' */

    uint8_t *tmp = (uint8_t*)realloc(used, new_cap * sizeof(uint8_t));
    if (!tmp) abort();                     /* memory allocation failed */

    /* Newly allocated bytes must start zero (unused). */
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
    used = tmp;
    used_cap = new_cap;
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1; never called with n==0 in tests */

    /* Compute the sequence up to term n if needed. */
    while (cached < n) {
        /* Find the smallest unused integer divisible by last_odd. */
        idx_t cand = (idx_t)last_odd;
        seq_t next = 0;

        while (true) {
            ensure_used(cand);
            if (!used[(size_t)cand]) {            /* unused?  Choose this value. */
                next = (seq_t)cand;
                used[(size_t)cand] = 1;           /* mark as used */
                break;
            }
            cand += (idx_t)last_odd;              /* next candidate multiple of last_odd */
        }

        /* Store the new term. */
        ensure_cache(cached + 1);
        cache[cached] = next;
        ++cached;

        /* Update the last odd decimal digit seen. */
        idx_t tmp = cand;
        uint8_t new_last = 0;
        while (tmp > 0) {
            uint8_t digit = (uint8_t)(tmp % 10);
            if (digit & 1) {                    /* odd digit found */
                new_last = digit;
                break;
            }
            tmp /= 10;
        }
        if (new_last != 0) last_odd = new_last;
    }

    return cache[n-1];
}
