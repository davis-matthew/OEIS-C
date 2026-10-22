/*
 * A384438  Composite numbers k such that ((2^k+1)/3)^k == 1 (mod k^2).
 * https://oeis.org/A384438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Order: Composite numbers k such that ((2^k+1)/3)^k == 1 (mod k^2).
   The function finds the n‑th such number by a brute‑force search with memoisation. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_k = 4;          /* next candidate k to check (>=4) */

    /* keep generating until we have at least n terms */
    while (cache_len < (size_t)n)
    {
        /* skip even k, since 2^k+1 is not divisible by 3 for even k */
        if (next_k % 2 == 0) {
            next_k++;
            continue;
        }

        /* composite test */
        if (is_prime(next_k)) {
            next_k++;
            continue;
        }

        /* MOD = 3 * k^2  (needed to compute (2^k + 1)/3 exactly) */
        seq_t k_sq   = (seq_t)next_k * (seq_t)next_k;   /* k^2  */
        seq_t mod_mul3 = k_sq * (seq_t)3;              /* 3*k^2 */

        /* 2^k  (mod 3*k^2) */
        seq_t pow2k_mod = mod_pow((seq_t)2, (uint64_t)next_k, mod_mul3);

        /* ((2^k + 1)/3)  (exact, because 3 divides 2^k+1) */
        seq_t t = (pow2k_mod + (seq_t)1) / (seq_t)3;    /* t < k^2 */

        /* test whether t^k == 1 (mod k^2) */
        seq_t power = mod_pow(t, (uint64_t)next_k, k_sq);
        if (power == (seq_t)1)
        {
            /* append to cache */
            if (cache_len + 1 > cache_cap) {
                /* double capacity (or set to 2 if empty) */
                size_t new_cap = cache_cap ? cache_cap * 2 : 2;
                seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_arr) exit(1); /* allocation failure */
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        next_k++;
    }

    return cache[n - 1];
}
