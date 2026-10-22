/*
 * A386099  Primes having only {1, 2, 6, 7} as digits.
 * https://oeis.org/A386099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 125, u32 n <= 48, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n-th prime whose decimal digits are only {1,2,6,7}
   by enumerating numbers with those digits and testing primality. */
#include <stdbool.h>          /* bool */
#include <stddef.h>           /* size_t, NULL */
#include <stdlib.h>           /* malloc, realloc, abort */
#include "../utils/is_prime_big.h"

static seq_t *prime_cache = NULL;     /* cached primes */
static size_t  cache_cap   = 0;        /* capacity of cache array    */
static size_t  cache_size  = 0;        /* number of primes stored    */

/* ensure cache has capacity for at least `needed` elements */
static void ensure_cache(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                     /* out of memory */
    prime_cache = tmp;
    cache_cap = new_cap;
}

/* generator state for numbers consisting only of digits 1,2,6,7 */
static idx_t cur_len = 1;                /* current digit length          */
static idx_t cur_idx = 0;                /* index within current length   */
static idx_t cur_pow4 = 4;               /* 4^cur_len (number of candidates of current length) */

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for index 0          */

    const int digit_map[4] = {1, 2, 6, 7};

    while (cache_size < (size_t)n) {
        /* build the next candidate number consisting only of allowed digits */
        seq_t num = 0;
        for (int pos = 0; pos < (int)cur_len; ++pos) {
            idx_t shift = ((idx_t)cur_len - 1 - (idx_t)pos) << 1; /* 2* (cur_len-1-pos)  */
            int di = (int)((cur_idx >> shift) & 3);
            num = num * 10 + digit_map[di];
        }

        if (is_prime_big(num)) {
            ensure_cache(cache_size + 1);
            prime_cache[cache_size++] = num;
        }

        /* advance to next candidate */
        cur_idx++;
        if (cur_idx == cur_pow4) {       /* finished current length      */
            cur_len++;
            cur_idx = 0;
            cur_pow4 <<= 2;               /* multiply by 4                */
        }
    }
    return prime_cache[n - 1];
}
