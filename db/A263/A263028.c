/*
 * A263028  Numbers n such that A002322(n) + 1 is a prime, where A002322 is
 *          Carmichael lambda.
 * https://oeis.org/A263028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n such that Carmichael λ(n)+1 is prime (OEIS A263028)
 * Range:     u8 n <= 222, u16 n <= 39742, u32 n <= 39743, u64 n <= 39743,
 *            u128 n <= 39743, big any n
 * Validated: standard, pass, 93 terms through n = 9573 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/isqrt.h"
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

/* ------------------------------------------------------------------ */
/* GCD for uint64_t                                                   */
/* ------------------------------------------------------------------ */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Prime list handling                                                 */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list = NULL;
static size_t    prime_cnt   = 0;
static uint64_t   prime_limit = 0;

static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_limit) return;
    if (prime_list != NULL) {
        free(prime_list);
        prime_list = NULL;
        prime_cnt = 0;
    }
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Carmichael λ(n) for n <= 2^64-1                                     */
/* ------------------------------------------------------------------ */
static uint64_t lambda_uint64(uint64_t n)
{
    if (n == 0) return 0;          /* undefined, not used */
    if (n == 1) return 1;          /* λ(1) = 1 */

    uint64_t result = 1;
    uint64_t m = n;
    uint64_t sqrt_n = isqrt(n) + 1;
    ensure_primes_upto(sqrt_n);

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= m; ++i) {
        uint64_t p = prime_list[i];
        if (m % p != 0) continue;
        unsigned int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        uint64_t part;
        if (p == 2) {
            if (exp == 1) part = 1;
            else if (exp == 2) part = 2;
            else part = 1ULL << (exp - 2);   /* 2^(e-2) */
        } else {
            uint64_t pow = 1;
            for (unsigned int k = 1; k < exp; ++k) pow *= p;  /* p^(exp-1) */
            part = pow * (p - 1);
        }
        uint64_t g = gcd_uint64(result, part);
        result = result / g * part;
    }

    if (m > 1) {  /* remaining prime factor */
        uint64_t p = m;
        uint64_t part = (p == 2) ? 1 : (p - 1);
        uint64_t g = gcd_uint64(result, part);
        result = result / g * part;
    }

    return result;
}

/* ------------------------------------------------------------------ */
/* Storage for the sequence                                            */
/* ------------------------------------------------------------------ */
static seq_t *seq_arr   = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static uint64_t next_nat = 1;      /* next integer to test */

static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = mem_resize(seq_t, seq_arr, new_cap);
    if (!tmp) abort();
    seq_arr = tmp;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Sequence function                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 */

    while (seq_len < (size_t)n) {
        uint64_t lam = lambda_uint64(next_nat);
        seq_t val = (seq_t)(lam + 1);
        if (is_prime_big(val)) {
            ensure_seq_capacity(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)next_nat;
        }
        ++next_nat;
    }
    return seq_arr[(size_t)n - 1];
}
