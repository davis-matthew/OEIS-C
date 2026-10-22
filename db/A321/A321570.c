/*
 * A321570  Least prime p such that p minus the multiplication of its digits
 *          is the n-th prime before p.
 * https://oeis.org/A321570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   The least prime p such that p minus the product of its digits *
 *            equals the n‑th prime before p (n ≥ 1). The function computes
 *            a(n) and * memoises results in a static table
 * Range:     u8 n <= 3, u16 n <= 133, u32 n <= 350, u64 n <= 350,
 *            u128 n <= 350, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    /* Static state for prime list and result cache */
    static seq_t *primes      = NULL;  /* array of found primes   */
    static size_t prime_cap  = 0;     /* current capacity */
    static size_t prime_cnt  = 0;     /* number of primes stored */
    static seq_t *cache      = NULL;  /* a(n) values, 1-indexed */
    static size_t cache_cap  = 0;     /* capacity of cache */
    static seq_t next_candidate = 3;   /* next odd number to test */
    static bool initialized = false;

    /* Initialise on first call */
    if (!initialized) {
        /* prime list */
        prime_cap = 16;
        primes = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        primes[0] = 2;
        prime_cnt = 1;

        /* cache for a(n) values */
        cache_cap = 16;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));

        initialized = true;
    }

    /* Ensure cache is large enough to hold n */
    if ((idx_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((idx_t)n >= new_cap)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Return already‑computed value */
    if (cache[n] != 0)
        return cache[n];

    /* Generate primes until a(n) is found */
    while (cache[n] == 0) {
        seq_t cand = next_candidate;
        bool prime_found = false;

        /* Find next prime */
        for (;;) {
            /* primality test using already found primes */
            size_t i;
            bool is_prime = true;
            for (i = 0; i < prime_cnt; ++i) {
                seq_t p = primes[i];
                seq_t sq = p * p;
                if (sq > cand)
                    break;
                if (cand % p == 0) {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime) {
                /* Append new prime to the list */
                if (prime_cnt == prime_cap) {
                    prime_cap <<= 1;
                    primes = (seq_t *)realloc(primes, prime_cap * sizeof(seq_t));
                }
                primes[prime_cnt++] = cand;

                /* Process this prime to update a(n) table */
                size_t idx_p = prime_cnt - 1;
                seq_t prod = digit_product(cand);
                seq_t q = cand - prod;
                if (q >= 2) {
                    /* binary search for q in primes */
                    size_t lo = 0, hi = prime_cnt - 1;
                    size_t idx_q = SIZE_MAX;
                    while (lo <= hi) {
                        size_t mid = lo + ((hi - lo) >> 1);
                        seq_t midv = primes[mid];
                        if (midv == q) {
                            idx_q = mid;
                            break;
                        } else if (midv < q) {
                            lo = mid + 1;
                        } else {
                            if (mid == 0)
                                break;
                            hi = mid - 1;
                        }
                    }

                    if (idx_q != SIZE_MAX) {
                        idx_t val_n = idx_p - idx_q;   /* n = idx_p - idx_q (>=1) */
                        if (val_n >= 1) {
                            /* Ensure cache large enough for val_n */
                            if ((idx_t)val_n >= cache_cap) {
                                size_t new_cap = cache_cap;
                                while ((idx_t)val_n >= new_cap)
                                    new_cap <<= 1;
                                cache = (seq_t *)realloc(cache,
                                                        new_cap * sizeof(seq_t));
                                memset(cache + cache_cap, 0,
                                       (new_cap - cache_cap) * sizeof(seq_t));
                                cache_cap = new_cap;
                            }
                            if (cache[val_n] == 0)
                                cache[val_n] = cand;
                        }
                    }
                }

                next_candidate = cand + 2;   /* next odd to test */
                break;                      /* prime processed, continue */
            } else {
                cand += 2;  /* skip even numbers */
            }
        }
    }

    return cache[n];
}
