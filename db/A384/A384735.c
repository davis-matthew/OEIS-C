/*
 * A384735  Numbers that are prime or end in a prime number (of any length).
 * https://oeis.org/A384735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer that is either prime or has a
 *            prime decimal suffix
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

static bool has_prime_suffix_or_is_prime(seq_t x) {
    /* Prime numbers themselves are accepted. */
    if (is_prime_big(x))
        return true;

    /* Check each suffix: last 1 digit, last 2 digits, ..., last d-1 digits. */
    seq_t mod = 10;
    while (mod <= x) {
        seq_t suffix = x % mod;
        if (suffix >= (seq_t)2 && is_prime_big(suffix))
            return true;
        mod *= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* stored sequence values */
    static size_t capacity = 0;   /* current allocation size */
    static size_t size = 0;       /* number of values stored */
    static seq_t last_checked = 1;/* last integer examined */

    if (n == 0)   /* undefined for n < 1, return 0 for safety */
        return (seq_t)0;

    /* Extend the cache until we have at least n terms. */
    while (size < n) {
        ++last_checked;                   /* next candidate integer */
        if (has_prime_suffix_or_is_prime(last_checked)) {
            if (size >= capacity) {
                capacity = capacity ? capacity * 2 : 128;
                cache = mem_resize(seq_t, cache, capacity);
                if (!cache) exit(1);          /* allocation failure */
            }
            cache[size++] = last_checked;   /* store the term */
        }
    }
    return cache[n - 1];
}
