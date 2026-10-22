/*
 * A399378  Numbers k that are multiples of A399376(k), where A399376(k) =
 *          {Smallest prime not dividing k'}^{the least significant nonzero
 *          digit in primorial base representation of k'}, and k' stands for
 *          A003415(k), the arithmetic derivative of k.
 * https://oeis.org/A399378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 4637, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A399378:  k such that k is a multiple of A399376(k).
   A399376(k) = [smallest prime not dividing k′]^lsd_primorial(k′),
   where k′ is the arithmetic derivative of k. 
   The function computes a(n) for n ≥ 1 by searching ks until
   the nth such number is found.
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"      /* is_prime(uint64_t)            */
#include "../utils/factor_u64.h"    /* factor_u64(uint64_t, …)      */
#include "../utils/pow_uint.h"     /* seq_t pow_uint(uint64_t, …)  */

/* types supplied by the build environment */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* ---------- helpers ------------------------------------------ */

/* arithmetic derivative of k  (k is an arbitrary precision integer) */
static seq_t arith_deriv(seq_t k)
{
    if (k <= 1) return 0;
    uint64_t n = (uint64_t)k;
    uint64_t primes[32], exps[32];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t d = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t div = k / primes[i];
        d += div * (seq_t)exps[i];
    }
    return d;
}

/* primes list sufficient for the primorial base representation
   up to the numbers we will handle (71! is astronomically large). */
static const uint64_t primes_list[20] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};

/* least‑significant non‑zero digit of x in primorial base. */
static unsigned lsd_primorial_digit(seq_t x)
{
    if (x == 0) return 0;
    seq_t n = x;
    seq_t weight = 1;
    size_t i = 0;
    while (weight <= n && i < 20) {
        seq_t div = n / weight;
        unsigned d = (unsigned)(div % primes_list[i]);
        if (d != 0) return d;
        weight *= primes_list[i];
        ++i;
    }
    return 0;
}

/* smallest prime not dividing x. */
static uint64_t smallest_prime_not_dividing(seq_t x)
{
    if (x == 0) return 2;
    uint64_t p = 2;
    while (1) {
        if ((x % p) != 0) return p;
        do { ++p; } while (!is_prime(p));
    }
}

/* ------------------------------------------------------------ */

/* main sequence function */
seq_t a(idx_t n)
{
    /* static storage to allow arbitrary call order */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t cache_len   = 0;
    static idx_t next_k      = 0;   /* next integer to test */

    /* enlarge cache if necessary to hold n terms */
    if (cache_len < n) {
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < (size_t)n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        /* generate terms until we have n of them */
        while (cache_len < n) {
            seq_t k = (seq_t)next_k;
            bool ok = false;
            if (next_k <= 1) {
                /* 0 and 1 satisfy the condition (k′ = 0) */
                ok = true;
            } else {
                seq_t kprime = arith_deriv(k);
                uint64_t sp = smallest_prime_not_dividing(kprime);
                unsigned digit = lsd_primorial_digit(kprime);
                seq_t a399 = pow_uint(sp, digit);
                ok = (k % a399) == 0;
            }
            if (ok) {
                cache[cache_len++] = k;
            }
            ++next_k;
        }
    }
    return cache[n - 1];
}
