/*
 * A386091  Primes having only {1, 2, 4, 5} as digits.
 * https://oeis.org/A386091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 96, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9674 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime that uses only the decimal
   digits {1,2,4,5}.  It is implemented by generating the numbers
   consisting of those digits in lexicographic order and testing
   them for primality; the result is cached so that calls with
   arbitrary indices are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static const uint8_t ALLOWED[4] = {1, 2, 4, 5};

/* Convert a base‑4 encoded offset into the decimal number whose
   digits are selected from ALLOWED.  The offset is treated as the
   representation in base 4 of a number with 'len' digits. */
static uint64_t number_from_offset(uint64_t len, uint64_t offset)
{
    uint8_t digits[64];               /* enough for all lengths used */
    uint64_t t = offset;
    for (int i = (int)len - 1; i >= 0; --i) {
        digits[i] = ALLOWED[t % 4];
        t /= 4;
    }

    uint64_t val = 0;
    for (uint64_t i = 0; i < len; ++i)
        val = val * 10 + digits[i];
    return val;
}

/* ------------------------------------------------------------------ */
/*                  Cache and on‑demand generation                    */
/* ------------------------------------------------------------------ */
static seq_t *prime_cache = NULL;     /* array of found primes, in order   */
static size_t prime_cap   = 0;        /* current allocated capacity       */
static size_t prime_len   = 0;        /* number of primes stored          */

static uint64_t current_len = 1;      /* length of the numbers being examined */
static uint64_t current_off = 0;      /* offset within the current length */

/* Ensure that prime_cache contains at least 'need' primes. */
static void ensure_cache(idx_t need)
{
    if (prime_len >= (size_t)need)
        return;

    if (prime_cap < (size_t)need) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        if (prime_cap < (size_t)need)
            prime_cap = (size_t)need;
        prime_cache = mem_resize(seq_t, prime_cache, prime_cap);
    }

    /* Generate primes until we have at least 'need' of them. */
    for (;;) {
        /* Move to next length when the current range is exhausted. */
        if (current_off >= (uint64_t)1 << (2 * current_len)) { /* 4^len = 2^(2*len) */
            ++current_len;
            current_off = 0;
            continue;
        }

        uint64_t cand = number_from_offset(current_len, current_off);
        ++current_off;

        /* Quick filter: only numbers ending in 1 (except 2 and 5) can be prime. */
        if (cand != 2 && cand != 5 && (cand % 10) != 1)
            continue;

        if (is_prime_big((seq_t)cand)) {
            prime_cache[prime_len++] = (seq_t)cand;
            if (prime_len >= (size_t)need)
                return;
        }
    }
}

/* ------------------------------------------------------------------ */
/*                          Exported function                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour for n < 1 is undefined; return 0 */
        return (seq_t)0;

    ensure_cache(n);
    return prime_cache[(size_t)n - 1];
}
