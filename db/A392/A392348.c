/*
 * A392348  Primes p such that the Chebyshev distance from 1 to p in the Ulam
 *          spiral is not a prime number.
 * https://oeis.org/A392348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime p such that floor((sqrt(p - 1) + 1) / 2) is not
 *            prime
 * Range:     u8 n <= 7, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* Static state for memoisation and progress */
    static seq_t *selected = NULL;   /* array of accepted primes */
    static size_t sel_cnt  = 0;      /* number of accepted primes found so far */
    static size_t sel_cap  = 0;      /* capacity of selected array */
    static seq_t  next_candidate = 2;   /* next integer to test for primality */

    /* Return already found term if available */
    if (n <= sel_cnt) {
        return selected[(size_t)(n - 1)];
    }

    /* Generate and test primes until we have n terms */
    while (sel_cnt < n) {
        seq_t cand = next_candidate;

        /* Test whether cand is prime */
        if (is_prime_big(cand)) {
            /* Compute k = (floor_sqrt(cand - 1) + 1) / 2 */
            seq_t r = floor_sqrt(cand - (seq_t)1);
            seq_t k = (r + (seq_t)1) / (seq_t)2;

            /* Accept cand if k is not prime (i.e., composite or 1) */
            if (!is_prime_big(k)) {
                /* Grow the selected array if necessary */
                grow_seq_array(&selected, &sel_cap, sel_cnt + (size_t)1);
                selected[sel_cnt++] = cand;
            }
        }

        /* Advance to the next candidate integer */
        ++next_candidate;
    }

    /* Return the requested term */
    return selected[(size_t)(n - 1)];
}
