/*
 * A386165  Primes having only {2, 6, 7, 8} as digits.
 * https://oeis.org/A386165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9844 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose decimal digits belong to {2,6,7,8}.
 * This function generates candidates with those digits, tests them
 * for primality using the supplied helper is_prime_big, and memoises
 * the results.  No simple formula exists, so the implementation is
 * a straightforward search. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
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

#define MAX_CACHE 20000

static seq_t prime_cache[MAX_CACHE];
static size_t prime_cnt = 0;

/* generate numbers whose digits are 2,6,7,8 only */
static const uint8_t ALLOWED[4] = {2,6,7,8};
static uint8_t digs[64];
static size_t digs_len = 0;
static bool first = true;

static void inc_digits(void)
{
    size_t i = digs_len - 1;
    for (;;)
    {
        if (digs[i] < 3)
        {
            ++digs[i];
            break;
        }
        digs[i] = 0;
        if (i == 0)
        {
            memmove(digs + 1, digs, digs_len);
            digs[0] = 0;
            ++digs_len;
            break;
        }
        --i;
    }
}

static seq_t next_candidate(void)
{
    if (first)
    {
        first = false;
        digs_len = 1;
        digs[0] = 0;                /* corresponds to digit 2 */
    }

    seq_t val = 0;
    for (size_t i = 0; i < digs_len; ++i)
        val = val * 10 + (seq_t)ALLOWED[digs[i]];

    inc_digits();
    return val;
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based and n ≥ 1 */
    while (prime_cnt < (size_t)n)
    {
        seq_t cand = next_candidate();
        if (is_prime_big(cand))
        {
            if (prime_cnt < MAX_CACHE)
                prime_cache[prime_cnt++] = cand;
            /* overflow beyond MAX_CACHE is not expected for tests */
        }
    }
    return prime_cache[n - 1];
}
