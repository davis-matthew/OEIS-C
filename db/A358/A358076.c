/*
 * A358076  Numbers that share at least 1 (decimal) digit with their largest
 *          proper divisor.
 * https://oeis.org/A358076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A358076: Numbers that share at least one decimal digit with
   their largest proper divisor.  The function generates the sequence
   by checking integers sequentially; it caches results so that any
   index n can be answered in O(n) work.  The definition is:
     a(n) = the n-th positive integer m such that m shares at least
            one decimal digit with its largest proper divisor. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"   /* helper: int_sqrt(idx_t) */

/* ---------- primality utilities ---------- */
static uint64_t *prime_list = NULL;   /* dynamic list of primes */
static size_t   prime_cap   = 0;
static size_t   prime_len   = 0;
static uint64_t prime_next  = 3;      /* next odd candidate to test */

/* ensure that prime_list contains all primes ≤ 'limit' */
static void prime_extend_to(uint64_t limit)
{
    if (prime_len == 0) {
        /* initialise with the only even prime */
        prime_list = (uint64_t *)realloc(prime_list, sizeof(uint64_t) * 4);
        prime_cap  = 4;
        prime_list[0] = 2;
        prime_len = 1;
    }

    while (prime_next <= limit) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if (p > prime_next / p) break;   /* p*p would overflow */
            if (prime_next % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_len == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 4;
                prime_list = (uint64_t *)realloc(prime_list,
                                                 prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_len++] = prime_next;
        }
        prime_next += 2;
    }
}

/* return the smallest prime factor of a 64‑bit integer > 1
   (if n is prime, returns n itself) */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if ((n & 1ULL) == 0) return 2;

    uint64_t limit = (uint64_t)int_sqrt(n);
    prime_extend_to(limit);
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p > limit) break;
        if (n % p == 0) return p;
    }
    return n;   /* n is prime */
}

/* ---------- digit mask for uint64_t -------------------------- */
static inline uint16_t digit_mask_uint64(uint64_t v)
{
    uint16_t m = 0;
    if (v == 0) return 1U;  /* digit 0 */
    while (v > 0) {
        m |= 1U << (v % 10);
        v /= 10;
    }
    return m;
}

/* ---------- sequence caching --------------------------------- */
static seq_t *seq_cache = NULL;  /* stored sequence values */
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_candidate = 2;   /* next integer to test */

/* return the n-th term of the sequence (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of offset, undefined */

    size_t target = (size_t)n;
    while (cache_len < target) {
        /* find largest proper divisor: n / smallest prime factor */
        uint64_t spf = smallest_prime_factor(next_candidate);
        uint64_t largest_proper = next_candidate / spf;

        /* test shared decimal digit */
        if (digit_mask_uint64(next_candidate) &
            digit_mask_uint64(largest_proper)) {
            /* extend cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_cache = (seq_t *)realloc(seq_cache,
                                              new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            seq_cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return seq_cache[target - 1];
}
