/*
 * A400172  Numbers k not divisible by any p^p and for which A003415(k) >=
 *          A276085(k), where A003415 is the arithmetic derivative and A276085
 *          is the primorial base log-function.
 * https://oeis.org/A400172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (k>=1) with no p^p dividing k and A003415(k) >=
 *            A276085(k); found by scanning k and caching terms
 * Range:     u8 n <= 27, u16 n <= 409, u32 n <= 28447, u64 n <= 28447,
 *            u128 n <= 28447, big any n
 * Validated: standard, pass, 81 terms through n = 9412 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const unsigned PRIMES_SMALL[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89};

static int qualifies(uint64_t k) {
    uint64_t rem = k;
    unsigned __int128 prod = 1, der = 0, sa = 0;
    for (size_t i = 0; i < sizeof(PRIMES_SMALL) / sizeof(PRIMES_SMALL[0]); i++) {
        unsigned p = PRIMES_SMALL[i];
        unsigned e = 0;
        while (rem % p == 0) {
            rem /= p;
            e++;
        }
        if (e) {
            if (e >= p) return 0;
            der += (unsigned __int128)(k / p) * e;
            sa += prod * e;
        }
        prod *= p;
    }
    if (rem > 1) return 0; /* prime factor >= 97: A276085 far exceeds derivative */
    return der >= sa;
}

static uint64_t *cache_terms = NULL;
static uint64_t cache_cnt = 0, cache_cap = 0, next_k = 1;

seq_t a(idx_t n) {
    while (cache_cnt < (uint64_t)n) {
        if (qualifies(next_k)) {
            if (cache_cnt == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 1024;
                cache_terms = (uint64_t *)realloc(cache_terms, cache_cap * sizeof(uint64_t));
            }
            cache_terms[cache_cnt++] = next_k;
        }
        next_k++;
    }
    return (seq_t)cache_terms[n - 1];
}
