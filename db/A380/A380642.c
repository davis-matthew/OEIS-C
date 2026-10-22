/*
 * A380642  Numbers k such that the total number of digits d in the numbers
 *          from 1 to k is even for each d from 0 to 9.
 * https://oeis.org/A380642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 148, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A380642: numbers k such that each decimal digit 0‑9 occurs an even number of times in 1..k.
   Computation: incremental search, keeping parity of counts for each digit. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* storage for discovered terms */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static unsigned char parity[10] = {0};/* parity of digit counts for current k */
static idx_t next_k = 1;             /* next integer to test (after initial 0) */

/* Ensure the cache has at least |needed| capacity, allocating or expanding as necessary. */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Generate terms until we have at least 'target' terms in the cache. */
static void generate_terms(idx_t target)
{
    /* Cache already has at least one term (0). */
    while (cache_len < target) {
        idx_t k = next_k++;
        /* Toggle parity bits for each decimal digit of k. */
        idx_t x = k;
        while (x > 0) {
            unsigned d = (unsigned)(x % 10);
            parity[d] ^= 1;
            x /= 10;
        }
        /* If all parity bits are zero, k is a valid term. */
        if (parity[0]==0 && parity[1]==0 && parity[2]==0 && parity[3]==0 &&
            parity[4]==0 && parity[5]==0 && parity[6]==0 && parity[7]==0 &&
            parity[8]==0 && parity[9]==0) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
    }
}

seq_t a(idx_t n)
{
    if (cache_len == 0) {            /* First call: initialize with 0 */
        ensure_cache_cap(1);
        cache[0] = (seq_t)0;
        cache_len = 1;
    }
    if (n < 1) return (seq_t)0;        /* undefined offset below 1, return 0 */
    if ((idx_t)cache_len < n)         /* need more terms */
        generate_terms(n);
    return cache[n-1];
}
