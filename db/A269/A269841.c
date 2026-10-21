/*
 * A269841  Integers n such that A000330(n) is the sum of 2 nonzero squares.
 * https://oeis.org/A269841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A269841: integers n such that A000330(n) = 1^2+2^2+...+n^2 is the sum of two
   non‑zero squares.  The function returns the nth element (offset 1).  */
   
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define PRIME_LIMIT 600000   /* enough for n up to about 10000 */

/* -------------------------------------------------------------------------- */
/* prime sieve – executed once on first use                                       */
static uint32_t primes[PRIME_LIMIT + 1];
static size_t  prime_cnt   = 0;
static bool    primes_done = false;

static void init_primes(void)
{
    if (primes_done) return;
    static bool is_composite[PRIME_LIMIT + 1];
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    primes_done = true;
}

/* -------------------------------------------------------------------------- */
/* integer square root using binary search (returns floor(sqrt(x)))               */
static unsigned int isqrt_uint64(uint64_t x)
{
    uint64_t low  = 0;
    uint64_t high = (1ULL << 32) - 1;   /* sqrt(UINT64_MAX) < 2^32 */
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        unsigned __int128 prod = (unsigned __int128)mid * mid;
        if (prod <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return (unsigned int)high;
}

/* -------------------------------------------------------------------------- */
/* test whether m can be written as x^2 + y^2 with x,y > 0                       */
static bool has_two_nonzero_squares(uint64_t m)
{
    /* 1. prime‑factor test: all p ≡ 3 (mod 4) must appear with even exponent */
    uint64_t temp = m;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if ((p & 3ULL) == 3ULL && (exp & 1))
                return false;
        }
    }
    if (temp > 1) {                /* remaining large prime factor */
        if ((temp & 3ULL) == 3ULL)
            return false;
    }

    /* 2. if m is not a perfect square -> representation exists with non‑zero squares */
    uint32_t r = isqrt_uint64(m);
    if ((uint64_t)r * r != m)
        return true;

    /* 3. m is a perfect square m = r^2   →  check if r contains a prime ≡ 1 (mod 4) */
    uint64_t t = r;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= t; ++i) {
        uint64_t p = primes[i];
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            if ((p & 3ULL) == 1ULL)
                return true;
        }
    }
    if (t > 1 && (t & 3ULL) == 1ULL)
        return true;

    return false;   /* all representations involve a zero square */
}

/* -------------------------------------------------------------------------- */
/* storage for found terms (dynamic array)                                      */
static idx_t *term_values = NULL;
static size_t term_len   = 0;
static size_t term_cap   = 0;
static idx_t  last_n     = 0;

static void ensure_terms_capacity(size_t new_len)
{
    if (new_len <= term_cap) return;
    size_t new_cap = term_cap ? term_cap * 2 : 128;
    while (new_cap < new_len) new_cap *= 2;
    idx_t *tmp = (idx_t *)realloc(term_values, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    term_values = tmp;
    term_cap = new_cap;
}

/* -------------------------------------------------------------------------- */
/* public function required by the build                                       */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset is 1, but guard against 0 */
    init_primes();

    /* generate terms until we have the nth value */
    while (term_len < n) {
        ++last_n;   /* next candidate n */

        /* A000330(last_n) = n(n+1)(2n+1)/6 */
        uint64_t nn   = (uint64_t)last_n;
        uint64_t m    = nn * (nn + 1) * (2 * nn + 1) / 6;

        if (has_two_nonzero_squares(m)) {
            ensure_terms_capacity(term_len + 1);
            term_values[term_len++] = last_n;
        }
    }

    return (seq_t)term_values[n - 1];
}
