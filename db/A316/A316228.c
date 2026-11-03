/*
 * A316228  Numbers whose Fermi-Dirac prime factorization sums to a
 *          Fermi-Dirac prime.
 * https://oeis.org/A316228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 4147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9896 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find numbers whose distinct Fermi‑Dirac prime factors sum to a Fermi‑Dirac prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
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

/* exponentiation for seq_t */
static inline seq_t pow_uint(seq_t base, unsigned exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            result = seq_mul(result, base);
        base = seq_mul(base, base);
        exp >>= 1U;
    }
    return result;
}

/* sum of the distinct FD primes in the factorization of n (n ≥ 2) */
static seq_t sum_fd_prime_factors(uint64_t n)
{
    seq_t sum = (seq_t)0;
    uint64_t temp = n;

    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            unsigned e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            for (unsigned bit = 0; (1ULL << bit) <= e; ++bit)
                if (e & (1U << bit))
                    sum += pow_uint((seq_t)p, 1U << bit);
        }
    }
    if (temp > 1)   /* remaining prime factor */
        sum += (seq_t)temp;   /* exponent 1 ⇒ 2^0 = 1 */

    return sum;
}

/* true if x is a Fermi‑Dirac prime: p^(2^k) with p prime, k >= 0 */
static bool is_fd_prime(seq_t x)
{
    if (x <= (seq_t)1)
        return false;
    if (is_prime_big(x))
        return true;

    seq_t t = x;
    while (true)
    {
        seq_t m = floor_sqrt(t);
        if (seq_mul(m, m) != t)
            break;
        t = m;
    }
    return is_prime_big(t);
}

/* sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t candidate = 2;   /* next integer to test */

    if (n == 0)
        return (seq_t)0;   /* safety for offset handling */

    while (cache_len < n)
    {
        uint64_t num = candidate;
        seq_t sum = sum_fd_prime_factors(num);

        if (is_fd_prime(sum))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *new_arr = mem_resize(seq_t, cache, new_cap);
                if (!new_arr)
                {
                    /* allocation failure: fallback to abort */
                    abort();
                }
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)num;
        }
        ++candidate;
    }
    return cache[n - 1];
}
