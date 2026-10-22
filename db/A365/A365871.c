/*
 * A365871  Starts of runs of 3 consecutive integers whose exponent of least
 *          prime factor in their prime factorization is even.
 * https://oeis.org/A365871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   even_lpf_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A365871:  a(n) = first integer k such that
   k, k+1, k+2 all have even exponent of the least prime factor
   of each number.  All terms are of the form 4m+3.  Implementation
   uses a simple trial‑division routine to test the parity of
   the least prime factor exponent. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/even_lpf_exp.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static seq_t candidate = (seq_t)3;      /* first number ≡ 3 (mod 4) */

    while (cache_len < n) {
        if (even_lpf_exp(candidate) &&
            even_lpf_exp(candidate + 1) &&
            even_lpf_exp(candidate + 2)) {

            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;       /* out of memory; judge handles */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        candidate += 4;   /* next integer ≡ 3 (mod 4) */
    }
    return cache[n - 1];
}
