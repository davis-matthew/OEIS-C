/*
 * A328770  Numbers in whose primorial base expansion any digit is at most
 *          half of the maximal allowed digit for that position.
 * https://oeis.org/A328770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose primorial base expansion digits * are at
 *            most (prime_k-1)/2 for each position k
 * Range:     u8 n <= 36, u16 n <= 2406, u32 n <= 9072, u64 n <= 9072,
 *            u128 n <= 9072, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the sequence on demand and caches computed
 * terms so that calls in any order are O(1) apart from the
 * initial build cost. */

#include <stdlib.h>
#include <stdbool.h>

/* Prime list sufficient for all numbers produced for n <= 10000
 * (more than enough for larger indices). */
static const unsigned int primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151
};
#define PRIMES_CNT (sizeof(primes)/sizeof(primes[0]))

static seq_t *cache      = NULL;     /* cached sequence terms      */
static idx_t  cache_size = 0;        /* number of cached terms     */
static idx_t  cache_cap  = 0;        /* allocated capacity         */
static seq_t  cur_val    = 0;        /* current candidate to test  */

static bool
term_ok(seq_t x)
{
    seq_t val = x;
    for (size_t k = 0; val > 0; ++k) {
        if (k >= PRIMES_CNT) {          /* safety; should never occur */
            return false;
        }
        seq_t p   = (seq_t)primes[k];
        seq_t digit = val % p;
        seq_t max_allowed = (p - 1) / 2;
        if (digit > max_allowed) {
            return false;
        }
        val /= p;
    }
    return true;
}

static void
grow_cache(idx_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1; arbitrary */

    if (cache_size < n) {
        grow_cache(n);
        while (cache_size < n) {
            if (term_ok(cur_val)) {
                cache[cache_size++] = cur_val;
            }
            cur_val++;
        }
    }
    return cache[n - 1];
}
