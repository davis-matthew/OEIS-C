/*
 * A359840  Numbers k that are the representation of primes in base 4 and in
 *          base 5.
 * https://oeis.org/A359840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A359840: numbers whose decimal digits, when interpreted as
   base‑4 and base‑5 numbers, are both prime.  The sequence is generated
   by scanning the positive integers, keeping only those with digits
   0–3, and testing the corresponding base‑4 and base‑5 values for
   primality.  The function a(n) returns the nth such number
   (offset starts at 1).  A simple cache is used so that calls are
   independent of the order of n. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

static seq_t *cache = NULL;     /* cache of found terms */
static size_t cache_size = 0;   /* number of terms stored          */
static size_t cache_cap   = 0;   /* allocated capacity               */
static seq_t next_candidate = 2; /* next decimal number to test      */

/* Double the cache capacity.  The initial capacity is 16. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
    if (!new_ptr) abort();
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Append a new term to the cache. */
static void append_cache(seq_t value)
{
    if (cache_size == cache_cap) grow_cache();
    cache[cache_size++] = value;
}

/* Test whether the candidate number has only digits 0‑3 and whether
   its base‑4 and base‑5 interpretations are both prime.  */
static bool candidate_ok(seq_t value)
{
    /* store decimal digits in reverse order */
    uint8_t digits[64];
    size_t len = 0;
    seq_t tmp = value;

    do {
        unsigned d = (unsigned)(tmp % 10);
        if (d > 3) return false;          /* digit out of range */
        digits[len++] = (uint8_t)d;
        tmp /= 10;
    } while (tmp);

    /* Convert to base‑4 and base‑5 values. */
    seq_t val4 = 0, val5 = 0;
    for (size_t i = len; i-- > 0;) {
        uint8_t d = digits[i];
        val4 = val4 * 4 + d;
        val5 = val5 * 5 + d;
    }

    return is_prime_big(val4) && is_prime_big(val5);
}

/* Generate terms until at least 'n' terms are cached. */
static void ensure_n(idx_t n)
{
    while (cache_size < (size_t)n) {
        if (candidate_ok(next_candidate))
            append_cache((seq_t)next_candidate);
        ++next_candidate;
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset starts at 1 */
    ensure_n(n);
    return cache[(size_t)(n - 1)];
}
