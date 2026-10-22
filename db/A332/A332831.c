/*
 * A332831  Numbers whose unsorted prime signature is neither weakly
 *          increasing nor weakly decreasing.
 * https://oeis.org/A332831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   brute‑force search – find the n‑th integer whose prime‑exponent
 *            list is neither weakly increasing nor weakly decreasing
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   factor_u64, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/factor_u64.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Static cache for sequence values (indexed from 0). */
static seq_t *cache = NULL;      /* pointer to array of seq_t values */
static size_t cache_len = 0;     /* number of stored terms                */
static size_t cache_cap = 0;     /* current capacity of cache array        */

/* Last integer examined during the search; starts just below the first
   candidate (which is 1). */
static uint64_t current_val = 1;

/* Append a new term to the cache, enlarging the array if necessary. */
static void cache_append(seq_t value)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        /* seq_realloc handles copying of class members if seq_t is a
           big‑integer class. */
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = value;
}

/* Return true iff n’s multiset of prime exponents is not monotone
   (neither weakly increasing nor weakly decreasing). */
static bool non_monotone_exponent_sequence(uint64_t n)
{
    uint64_t primes_buf[64];
    uint64_t exps_buf[64];
    size_t cnt = 0;

    factor_u64(n, primes_buf, exps_buf, &cnt);

    if (cnt <= 1)
        return false;            /* single exponent is always monotone */

    bool inc = true, dec = true;
    for (size_t i = 1; i < cnt; ++i) {
        if (exps_buf[i - 1] > exps_buf[i])
            dec = false;
        if (exps_buf[i - 1] < exps_buf[i])
            inc = false;
    }
    return !inc && !dec;         /* true only if neither monotone */
}

/* O(n) enumeration – works for any build type. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined for offset < 1 */

    while (cache_len < n) {
        ++current_val;

        if (non_monotone_exponent_sequence(current_val))
            cache_append((seq_t)current_val);
    }
    return cache[n - 1];
}
