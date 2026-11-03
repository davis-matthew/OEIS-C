/*
 * A380983  Numbers m whose sum of unitary divisors different from 1 and m is
 *          prime.
 * https://oeis.org/A380983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m such that sigma^*(m)-m-1 is prime *
 *            (sigma^*(m) = product over prime factors p^e of (1+p^e)) *
 *            Computed by incremental search with memoization
 * Range:     u8 n <= 43, u16 n <= 3187, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9711 (2026-11-03)
 * Helpers:   build_primes_upto, ipow_u64_idx, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *seq_cache = NULL;   /* cached terms */
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

static uint64_t *primes     = NULL; /* primes for factorization */
static size_t    primes_cnt = 0;

static bool initialized = false;
static uint64_t next_candidate = 2; /* next m to test */

/* Ensure helper tables are initialized once */
static void init_globals(void)
{
    if (initialized) return;
    /* Build a prime table up to a safe bound (1e6) for factorization */
    build_primes_upto(1000000, &primes, &primes_cnt);
    seq_cap = 32;
    seq_cache = mem_new(seq_t, seq_cap);
    initialized = true;
}

/* Store a value into the cache, expanding if necessary */
static void push_term(seq_t val)
{
    if (seq_len >= seq_cap) {
        seq_cap = seq_cap == 0 ? 32 : seq_cap * 2;
        seq_cache = mem_resize(seq_t, seq_cache, seq_cap);
    }
    seq_cache[seq_len++] = val;
}

/* Factorize n via trial division using the primes list */
static void unitary_sigma(uint64_t n, seq_t *sigma_out)
{
    seq_t sigma = 1;
    uint64_t remaining = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= remaining; ++i) {
        uint64_t p = primes[i];
        if (remaining % p == 0) {
            uint8_t exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                exp++;
            }
            /* compute (1 + p^exp) */
            seq_t pe = ipow_u64_idx(p, exp);
            seq_t term = pe + (seq_t)1;
            sigma *= term;
        }
    }
    if (remaining > 1) {   /* remaining is prime */
        seq_t pe = (seq_t)remaining;
        seq_t term = pe + (seq_t)1;
        sigma *= term;
    }
    *sigma_out = sigma;
}

seq_t a(idx_t n)
{
    init_globals();
    if (n <= 0) return (seq_t)0; /* undefined, return 0 */

    while (seq_len < (size_t)n) {
        uint64_t m = next_candidate++;
        seq_t sigma;
        unitary_sigma(m, &sigma);
        seq_t diff = sigma - (seq_t)m - (seq_t)1;
        if (diff > (seq_t)1 && is_prime_big(diff))
            push_term((seq_t)m);
    }
    return seq_cache[n-1];
}
