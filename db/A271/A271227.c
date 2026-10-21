/*
 * A271227  Number of solutions to y^2 == x^3 + 17 (mod p) as p runs through
 *          the primes.
 * https://oeis.org/A271227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6498, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of a(n) = number of solutions to y^2 = x^3 + 17 (mod prime(n)) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"        /* get_prime(idx_t) returns the nth prime */
#include "../utils/jacobi_symbol.h"   /* int jacobi_symbol(uint64_t a, uint64_t n); */

seq_t a(idx_t n)
{
    /* ----------------- caching ------------------------------------------ */
    static seq_t *cache = NULL;      /* 1‑based: cache[n-1] holds a(n)          */
    static idx_t cached  = 0;        /* number of entries cached so far        */

    if (n <= cached && cache[n - 1] != 0)
        return cache[n - 1];

    /* ----------------- prime of index n -------------------------------- */
    uint32_t p = (uint32_t)get_prime(n);    /* p fits into 32 bits               */

    /* ----------------- compute number of solutions --------------------- */
    seq_t count;

    /* Silverman theorem */
    if (p == 2) {
        count = (seq_t)p;
    } else if (p == 3) {
        count = (seq_t)p;        /* 3 solutions for p = 3 */
    } else if (p % 3U == 2U) {   /* p ≡ 2 (mod 3)         */
        count = (seq_t)p;
    } else {                     /* p ≡ 1 (mod 3)         */
        count = 0;
        for (uint32_t x = 0; x < p; ++x) {
            uint64_t t = (uint64_t)x * x;          /* x^2  */
            t %= p;
            t = (t * x) % p;                        /* x^3  */
            t += 17;
            if (t >= p) t -= p;
            uint64_t v = t;                         /* 0 ≤ v < p    */

            if (v == 0) {
                count += 1;                        /* y = 0 only    */
            } else {
                int leg = jacobi_symbol(v, p);      /* Legendre symbol */
                if (leg == 1)
                    count += 2;                    /* two square roots */
            }
        }
    }

    /* ----------------- store in cache --------------------------------- */
    if (n > cached) {
        /* realloc may return void*; cast to seq_t* for C++ */
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        for (idx_t i = cached; i < n; ++i)
            cache[i] = 0;
        cached = n;
    }
    cache[n - 1] = count;

    return count;
}
