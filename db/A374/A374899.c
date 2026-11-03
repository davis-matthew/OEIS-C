/*
 * A374899  a(n) is the least semiprime that starts with n.
 * https://oeis.org/A374899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = the least semiprime that starts with the decimal digits
 *            of n
 * Range:     u8 n <= 6, u16 n <= 655, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8878 (2026-11-03)
 * Helpers:   build_primes, ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* prime sieve limit – chosen to cover the square roots of all numbers searched */
static const idx_t PRIME_LIMIT = 4000000ULL;

/* static storage for the prime list */
static idx_t *primes = NULL;
static size_t prime_cnt = 0;

/* helper: test whether a number is a semiprime (product of exactly two primes, possibly equal) */
static bool is_semiprime(seq_t m) {
    if (m < 4) /* 4 is the smallest semiprime */
        return false;

    /* get integer square root of m */
    seq_t sqrt_m_seq = floor_sqrt(m);
    idx_t sqrt_m = (idx_t)sqrt_m_seq;

    /* search for a small factor */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];

        /* stop if p > sqrt(m) */
        if ((seq_t)p * (seq_t)p > m)
            break;

        if (m % (seq_t)p == 0) {
            seq_t q = m / (seq_t)p;           /* the co‑factor */
            if (q <= 1)                      /* should never happen */
                return false;
            /* q must be prime for m to be a semiprime */
            return is_prime_big(q);
        }
    }

    /* no divisor found ⇒ m is prime */
    return false;
}

/* main sequence function */
seq_t a(idx_t n) {
    /* initialise the prime list on first call */
    if (!primes) {
        build_primes(PRIME_LIMIT, &primes, &prime_cnt);
    }

    seq_t n_seq = (seq_t)n;

    for (idx_t k = 0; ; ++k) {
        seq_t powk = pow10(k);
        seq_t lower = n_seq * powk;
        seq_t upper = ((seq_t)(n + 1) * powk) - 1;

        /* iterate over all numbers that start with the decimal digits of n */
        for (seq_t m = lower; ; ++m) {
            if (m > upper)            /* reached the end of this block */
                break;
            if (is_semiprime(m))
                return m;            /* found the least semiprime with the desired prefix */
        }
    }
}
