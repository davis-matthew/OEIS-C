/*
 * A392219  Primes p such that the Chebyshev distance from 1 to p in the Ulam
 *          spiral is a prime number.
 * https://oeis.org/A392219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that floor((sqrt(p-1)+1)/2) is prime
 *            (computed by searching primes and testing the derived value)
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include <stdint.h>
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

/* ------------------------------------------------------------------ */
/*  Static state                                                    */
/* ------------------------------------------------------------------ */
static idx_t *prime_list      = NULL;   /* primes found so far */
static size_t prime_count     = 0;        /* number of primes stored   */
static size_t prime_cap       = 0;        /* allocated capacity       */
static idx_t next_candidate   = 2;        /* next number to test */

static seq_t *seq_vals        = NULL;   /* qualifying sequence values */
static size_t seq_len         = 0;        /* number of values stored  */
static size_t seq_cap         = 0;        /* allocated capacity       */

/* ------------------------------------------------------------------ */
/*  Utilities for dynamic arrays                                     */
/* ------------------------------------------------------------------ */
static void ensure_prime_cap(size_t needed) {
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = new_cap;
}

static void ensure_seq_cap(size_t needed) {
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = mem_resize(seq_t, seq_vals, new_cap);
    if (!tmp) abort();
    seq_vals = tmp;
    seq_cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Prime testing using the stored prime list                         */
/* ------------------------------------------------------------------ */
static bool is_prime_using_list(idx_t x) {
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > x / p) break;          /* avoid overflow */
        if (x % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Generate the next prime number and append it to prime_list      */
/* ------------------------------------------------------------------ */
static idx_t next_prime(void) {
    while (true) {
        /* Skip even numbers after 2 */
        if (next_candidate > 2 && (next_candidate & 1) == 0) {
            ++next_candidate;
            continue;
        }
        if (is_prime_using_list(next_candidate)) {
            ensure_prime_cap(prime_count + 1);
            prime_list[prime_count++] = next_candidate;
            idx_t prime = next_candidate;
            ++next_candidate;
            return prime;
        }
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/*  Public routine: a(idx_t n)                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* safety for offset 0 */

    /* Generate sequence values until we have at least n terms */
    while (seq_len < n) {
        idx_t p_val = next_prime();           /* next prime candidate */
        seq_t p = (seq_t)p_val;               /* cast to seq_t */

        /* Compute m = floor((sqrt(p-1)+1)/2) */
        seq_t t = floor_sqrt(p - 1);
        seq_t m = (t + 1) >> 1;               /* divide by 2 safely */

        /* Test whether m itself is prime */
        if (is_prime_big(m)) {
            ensure_seq_cap(seq_len + 1);
            seq_vals[seq_len++] = p;
        }
    }
    return seq_vals[n - 1];
}
