/*
 * A372770  Primes in A284798.
 * https://oeis.org/A372770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   Enumerates primes that are antipalindromic in base 3 (OEIS
 *            A372770)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 2007, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

/* static cache of discovered primes */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of primes stored */
static size_t cache_cap = 0;   /* capacity of the cache */

/* state for generating candidates */
static idx_t cur_k = 1;            /* current number of digits on one side (k) */
static idx_t cur_index = 0;        /* index of the current combination (0 … 3^k-1) */
static idx_t cur_k_total = 3;      /* 3^cur_k, number of combinations for this k */

/* ensure that at least n primes are stored in the cache */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    /* allocate an on‑stack array that accommodates the maximum possible k.
       k will be small (≤ 20 for n up to 10000) so 64 bytes is safe. */
    uint8_t left[64];

    while (cache_len < (size_t)n) {
        /* advance to the next number of digits if all combinations
           of the current length have been processed */
        if (cur_index >= cur_k_total) {
            cur_k++;
            cur_k_total *= 3;      /* 3^k */
            cur_index = 0;
        }

        /* decode cur_index into base‑3 digits of length cur_k (most
           significant first).  A copy is used so that cur_index remains
           unchanged for the next iteration. */
        idx_t tmp = cur_index;
        for (int pos = (int)cur_k - 1; pos >= 0; pos--) {
            left[pos] = (uint8_t)(tmp % 3);
            tmp /= 3;
        }

        /* skip numbers with a leading zero – they would have a shorter
           length and thus already been considered. */
        if (left[0] != 0) {
            /* construct the antipalindromic number:
               left part, middle digit 1, right part (2 - left[i]) */
            seq_t val = 0;
            for (idx_t i = 0; i < cur_k; i++)
                val = val * 3 + (seq_t)left[i];
            val = val * 3 + (seq_t)1;
            for (int i = (int)cur_k - 1; i >= 0; i--)
                val = val * 3 + (seq_t)(2u - left[i]);

            /* test for primality and store if it is a prime */
            if (isPrimeGeneric(val)) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) { /* handle allocation failure */
                        free(cache);
                        cache = NULL;
                        cache_len = 0;
                        cache_cap = 0;
                        return;
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = val;
            }
        }

        cur_index++; /* move to the next combination of the current length */
    }
}

/* public interface -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                       /* offset is 1; behaviour for 0 is irrelevant */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
