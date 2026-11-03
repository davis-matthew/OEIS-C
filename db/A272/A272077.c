/*
 * A272077  Primes of the form abs(7*k^2 - 371*k + 4871) in order of
 *          increasing nonnegative values of k.
 * https://oeis.org/A272077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form |7*k^2 - 371*k + 4871| for k ≥
 *            0, returned in order of increasing k
 * Range:     u8 none, u16 n <= 96, u32 n <= 3530, u64 n <= 3530,
 *            u128 n <= 3530, big any n
 * Validated: standard, pass, 63 terms through n = 3208 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t found = 0;
    static idx_t next_k = 0;

    if (n == 0)  /* undefined, but return 0 for safety */
        return 0;

    /* Build cached values up to the requested index */
    while (found < n)
    {
        /* Compute the quadratic expression as a signed 64‑bit value.
           For sequences up to ~10⁴ terms the intermediate values fit
           comfortably in a signed 64‑bit integer. */
        int64_t k  = (int64_t)next_k;
        int64_t v  = 7 * k * k - 371 * k + 4871;
        if (v < 0) v = -v;
        seq_t val = (seq_t)v;

        /* Test for primality */
        if (is_prime_big(val))
        {
            /* Grow cache if needed */
            if (found == cap)
                grow_seq(&cache, &cap, found + 1);
            cache[found++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
