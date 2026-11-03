/*
 * A253149  Primes >= 256 that remain primes when the digits are reversed in
 *          base 256.
 * https://oeis.org/A253149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1229, u32 n <= 7110, u64 n <= 7110,
 *            u128 n <= 7110, big any n
 * Validated: standard, pass, 64 terms through n = 6932 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253149:
 *   primes p ≥ 256 for which reversing the 8‑bit words of p (i.e.
 *   the base‑256 representation) yields another prime.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t rev_bytes(seq_t x)
{
    seq_t rev = 0;
    const seq_t mask = (seq_t)0xFF;
    while (x != (seq_t)0) {
        rev = (rev << 8) | (x & mask);
        x >>= 8;
    }
    return rev;
}

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n==0 */

    /* Static state that persists across calls */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_count = 0;
    static idx_t last_checked = 256; /* next candidate will start after this */

    /* Ensure the cache can hold at least n terms */
    if (cache_count < n) {
        /* Grow the cache appropriately (grow_seq ensures capacity >= need) */
        grow_seq(&cache, &cache_cap, n);

        /* Find more primes until we have n terms */
        while (cache_count < n) {
            /* Get the next prime ≥ last_checked */
            idx_t p64 = next_prime(last_checked);
            seq_t p = (seq_t)p64;

            /* Test whether the reversed 8‑bit word version is also prime */
            if (is_prime_big(rev_bytes(p))) {
                /* Store the prime that satisfies the condition */
                cache[cache_count++] = p;
            }

            /* Set up for the next candidate: start just after the prime we
               have examined. If p64==~(idx_t)0 the next candidate would
               overflow; in that unlikely case we stop. */
            if (p64 == (idx_t)-1) break;          /* wrap‑around, safety check */
            last_checked = p64 + 1;
        }
    }

    return cache[n - 1];
}
