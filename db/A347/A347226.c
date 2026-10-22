/*
 * A347226  Safe primes (A005385) that are half-period primes (A097443).
 * https://oeis.org/A347226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   append_candidate, factor_u64, is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search successive primes p, testing whether p is a safe prime
   ((p‑1)/2 is also prime) and a half‑period prime (the multiplicative order of
   10 modulo p is (p‑1)/2). The function is memoized so each index can be
   requested in any order. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"
#include "../utils/factor_u64.h"
#include "../utils/append_candidate.h"

static seq_t      *cache   = NULL;   /* array of found terms */
static size_t      cache_len  = 0;   /* number of stored terms */
static size_t      cache_cap  = 0;   /* current capacity of cache */
static uint64_t    next_pr   = 3;    /* next integer to test for primality */

/* extend the cache until it contains at least 'needed' terms */
static void extend_seq(idx_t needed)
{
    while (cache_len < (size_t)needed) {
        /* find next prime candidate */
        while (!is_prime_big((seq_t)next_pr)) {
            ++next_pr;
        }

        seq_t p  = (seq_t)next_pr;
        uint64_t sp = (next_pr - 1) / 2;          /* (p-1)/2  */
        if (!is_prime_big((seq_t)sp)) {           /* not a safe prime */
            ++next_pr;
            continue;
        }

        /* check half‑period property: 10^(sp) ≡ 1 (mod p) ? */
        if (mod_pow((seq_t)10, sp, p) != 1) {
            ++next_pr;
            continue;
        }

        /* compute the minimal order dividing sp */
        uint64_t ord = sp;
        uint64_t pf[64], pe[64];
        size_t pcnt;
        factor_u64(sp, pf, pe, &pcnt);
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t r = pf[i];
            while (ord % r == 0) {
                if (mod_pow((seq_t)10, ord / r, p) == 1) {
                    ord /= r;
                } else {
                    break;
                }
            }
        }

        if (ord == sp) {                         /* p is a half‑period prime */
            append_candidate(&cache, &cache_len, &cache_cap, p);
        }
        ++next_pr;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */
    if ((size_t)n > cache_len) extend_seq(n);
    return cache[(size_t)n-1];
}
