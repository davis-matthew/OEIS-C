/*
 * A270802  Primes p of the form 14*k+1 for which there is a solution to x^7
 *          == 2 mod p.
 * https://oeis.org/A270802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find primes p ≡ 1 (mod 14) for which 2 is a 7‑th power residue modulo p.
   The function returns the n‑th such prime (a(1) = 631). */

#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_candidate = 15;   /* 14·1 + 1 */

    while (cache_len < (size_t)n) {
        uint64_t p = next_candidate;
        next_candidate += 14;              /* advance to next 14k+1 number */

        if (!is_prime(p))
            continue;                     /* skip composites */

        uint64_t exp = (p - 1) / 7;
        seq_t  val   = mod_pow((seq_t)2, exp, (seq_t)p);

        if (val == (seq_t)1) {            /* 2 is a 7‑th power residue */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
