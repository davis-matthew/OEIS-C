/*
 * A386075  Primes having only {0, 4, 7, 9} as digits.
 * https://oeis.org/A386075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth prime whose decimal representation uses only the
 *            digits {0,4,7,9}. Computed by generating all such numbers in
 *            increasing length order and testing for primality
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 45, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *prime_cache = NULL;
static size_t cache_cap = 0;
static idx_t prime_cnt   = 0;
static idx_t next_len    = 1;  /* next number length to generate */

/* Ensure the cache has room for at least 'need' elements. */
static void ensure_cache_cap(idx_t need)
{
    if ((size_t)need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_cache = tmp;
        cache_cap = new_cap;
    }
}

/* Recursively generate all numbers of 'len' digits formed by
   digits {0,4,7,9} (leading digit cannot be 0) and test them for
   primality.  Found primes are appended to the cache in order. */
static void gen_len_recursive(idx_t pos, seq_t acc, idx_t len)
{
    if (pos == len) {
        if (is_prime_big(acc)) {
            ensure_cache_cap(prime_cnt + 1);
            prime_cache[prime_cnt++] = acc;
        }
        return;
    }

    if (pos == 0) { /* first digit: 4,7,9 */
        static const uint8_t digits0[3] = {4,7,9};
        for (size_t i = 0; i < 3; ++i)
            gen_len_recursive(pos + 1, acc * 10 + digits0[i], len);
    } else {          /* following digits: 0,4,7,9 */
        static const uint8_t digits1[4] = {0,4,7,9};
        for (size_t i = 0; i < 4; ++i)
            gen_len_recursive(pos + 1, acc * 10 + digits1[i], len);
    }
}

/* Generate primes until we have at least 'target' terms. */
static void build_up_to(idx_t target)
{
    while (prime_cnt < target) {
        gen_len_recursive(0, (seq_t)0, next_len);
        next_len++;  /* all numbers of this length have been processed */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    build_up_to(n);
    return prime_cache[n - 1];
}
