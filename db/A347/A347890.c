/*
 * A347890  Odd numbers k such that sigma(k) > 2*k and A003415(sigma(k)) < k,
 *          where A003415 is the arithmetic derivative, and sigma is the sum
 *          of divisors function.
 * https://oeis.org/A347890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A347890: odd numbers k such that sigma(k) > 2*k and
   arithmetic_derivative(sigma(k)) < k.  The function returns the n‑th
   term (1‑based).  The implementation uses 64‑bit arithmetic and
   caches the first 10 000 terms only, which is sufficient for all
   requested indices.  It works for both 64‑bit integer builds and
   arbitrary‑precision builds where `seq_t` is a class that can be
   constructed from an unsigned 64‑bit integer. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* ---------- prime sieve for factorisation -------------------------------- */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static void init_primes(void) {
    if (prime_list) return;
    /* enough for all k examined (m² ≤ 1e12) */
    build_primes_upto(4000000ULL, &prime_list, &prime_count);
}

static void factor_uint64(uint64_t n, uint64_t *prim, uint8_t *exp, size_t *cnt) {
    init_primes();
    *cnt = 0;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= n; ++i) {
        if (n % prime_list[i] == 0) {
            uint8_t e = 0;
            do {
                n /= prime_list[i];
                ++e;
            } while (n % prime_list[i] == 0);
            prim[*cnt] = prime_list[i];
            exp[*cnt]  = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        prim[*cnt] = n;
        exp[*cnt]  = 1;
        ++(*cnt);
    }
}

/* ---------- sum of divisors --------------------------------------------- */
static uint64_t sigma_uint64(uint64_t n) {
    uint64_t prim[64];
    uint8_t  exp[64];
    size_t cnt;
    factor_uint64(n, prim, exp, &cnt);
    __uint128_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = prim[i];
        uint8_t  e = exp[i];
        __uint128_t pow = 1;
        for (uint8_t k = 0; k < e + 1; ++k) pow *= p;                 /* p^(e+1) */
        __uint128_t term = (pow - 1) / (p - 1);                       /* (p^(e+1)-1)/(p-1) */
        res *= term;
        if (res > (__uint128_t)UINT64_MAX) return UINT64_MAX;
    }
    return (uint64_t)res;
}

/* ---------- arithmetic derivative ---------------------------------------- */
static unsigned __int128 derivative_uint64(uint64_t n) {
    uint64_t prim[64];
    uint8_t  exp[64];
    size_t cnt;
    factor_uint64(n, prim, exp, &cnt);
    unsigned __int128 d = 0;
    for (size_t i = 0; i < cnt; ++i) {
        d += (unsigned __int128)(n / prim[i]) * exp[i];
    }
    return d;
}

/* ---------- internal cache ---------------------------------------------- */
#define CACHE_MAX 10010      /* enough for all requested n ≤ 10 000 */
static seq_t cache[CACHE_MAX];
static idx_t cache_len = 0;
static uint64_t current_m = 1;      /* odd m to test */

/* ---------- public sequence accessor ------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    while (cache_len < n) {
        if ((current_m & 1ULL) == 0) ++current_m;   /* ensure odd */
        uint64_t m = current_m;
        uint64_t k = m * m;                         /* k = m² fits in 64‑bit */

        uint64_t sigma_k = sigma_uint64(k);
        if (sigma_k > 2 * k) {
            unsigned __int128 deriv = derivative_uint64(sigma_k);
            if (deriv < k) {
                cache[cache_len++] = (seq_t)k;
            }
        }
        current_m += 2;                            /* next odd */
    }
    return cache[n - 1];
}
