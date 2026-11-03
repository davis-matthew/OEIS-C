/*
 * A243457  Primes p such that (p reversed)-2 is also a prime.
 * https://oeis.org/A243457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 244, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 952 (2026-11-03)
 * Helpers:   is_prime_u64, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes p such that reverse(p) - 2 is also prime.
   The function maintains a static cache so that calls in any order
   are answered correctly. The cache is resized using seq_realloc
   to work with both C and C++ arbitrary‑precision builds. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/reverse_digits.h"
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* The offset for this sequence is 1, so n>=1. */
    if (n == 0) return (seq_t)0;

    /* Static cache of found special primes. */
    static seq_t *special = NULL;
    static idx_t count = 0;         /* number of special primes found */
    static idx_t last_checked = 5;  /* next candidate to test */
    static size_t cap = 0;          /* capacity of the cache array */

    /* Extend the cache until it contains at least n elements. */
    if (n > count) {
        idx_t candidate = last_checked;
        while (count < n) {
            /* Skip non‑primes. */
            while (!is_prime_big((seq_t)candidate)) {
                candidate++;
            }
            /* candidate is a prime. */
            seq_t rev = reverse_digits((seq_t)candidate);
            seq_t rev_minus = rev - (seq_t)2;
            if (rev_minus >= (seq_t)2 && is_prime_big(rev_minus)) {
                /* Store the special prime. */
                if (count >= cap) {
                    size_t newcap = cap ? cap * 2 : 8;
                    special = seq_realloc(special, cap, newcap);
                    cap = newcap;
                }
                special[count++] = (seq_t)candidate;
            }
            candidate++;
            last_checked = candidate;
        }
    }
    return special[n - 1];
}
