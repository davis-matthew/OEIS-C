/*
 * A281093  Primes having only {3, 4, 7, 9} as digits.
 * https://oeis.org/A281093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n-th prime whose decimal digits belong only to {3,4,7,9}
 * Range:     u8 n <= 8, u16 n <= 166, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9737 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
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

/* Check that all decimal digits of x belong to the allowed set. */
static inline bool allowed_digits(uint64_t x)
{
    while (x != 0) {
        uint8_t d = (uint8_t)(x % 10);
        if (d != 3 && d != 4 && d != 7 && d != 9)
            return false;
        x /= 10;
    }
    return true;
}

/* Cache of found terms. */
static seq_t *seq_cache = NULL;
static size_t   seq_cap  = 0;
static size_t   seq_len  = 0;
static uint64_t next_candid = 3;

/* Ensure the cache can hold at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (seq_cap >= new_cap)
        return;
    size_t cap = seq_cap == 0 ? 64 : seq_cap * 2;
    while (cap < new_cap)
        cap *= 2;
    seq_t *tmp = mem_resize(seq_t, seq_cache, cap);
    if (!tmp)
        abort();
    seq_cache = tmp;
    seq_cap   = cap;
}

/* Main function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n==0, caller should not use this */

    while (seq_len < (size_t)n) {
        if (allowed_digits(next_candid) && is_prime_big((seq_t)next_candid)) {
            grow_cache(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_candid;
        }
        /* Move to the next odd candidate. */
        if (next_candid == 2)
            next_candid = 3;
        else
            next_candid += 2;   /* only odd numbers */
    }
    return seq_cache[(size_t)n - 1];
}
