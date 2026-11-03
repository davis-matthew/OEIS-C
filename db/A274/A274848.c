/*
 * A274848  Numbers k such that k^2 is expressible in just one way as
 *          (p+1)(q+1) where p, q are distinct primes.
 * https://oeis.org/A274848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8414 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A274848: k such that k² = (p+1)(q+1) with distinct primes p,q
   only in one way.  The function slowly builds the list of
   qualifying k using trial‑division of each candidate. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <string.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

/* ----- helper: integer square root for uint64_t ----- */
static inline idx_t isqrt_uint64(idx_t n)
{
    idx_t low = 0, high = (idx_t)1 << 32;          /* high^2 <= 2^64-1 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)   low = mid + 1;
        else          high = mid - 1;
    }
    return high;                                   /* floor(sqrt(n)) */
}

/* ----- variabes to keep the sequence across calls ----- */
static bool initialized = false;
static seq_t *seq = NULL;          /* array of found k's          */
static size_t seq_len = 0;         /* number of elements stored   */
static size_t seq_cap = 0;         /* current capacity            */
static idx_t next_k = 2;           /* smallest candidate to test  */

/* ----- primes table for factorisation ----- */
static uint64_t *prime_list = NULL;
static size_t  prime_cnt  = 0;
static uint64_t prime_max  = 0;

/* Ensure all primes up to 'bound' are available */
static void ensure_primes_up_to(uint64_t bound)
{
    if (bound <= prime_max) return;
    uint64_t *tmp  = NULL;
    size_t   tcnt  = 0;
    build_primes_upto(bound, &tmp, &tcnt);
    free(prime_list);
    prime_list = tmp;
    prime_cnt  = tcnt;
    prime_max  = bound;
}

/* ----- data for divisor enumeration of current_k^2 ----- */
static idx_t current_k;
static idx_t current_sq;
static int   count_pairs;
static int   fp_cnt;
static int   fp_primes[32];
static int   fp_exps[32];

/* Recursive generation of divisors of current_k^2 not exceeding current_k */
static void gen_divs(int idx, idx_t val)
{
    if (idx == fp_cnt) {
        if (val > current_k || val <= 1) return;
        idx_t a_minus_1 = val - 1;
        if (!isPrimeGeneric((seq_t)a_minus_1)) return;
        idx_t b = current_sq / val;
        if (b == val) return;                     /* distinct primes needed */
        idx_t b_minus_1 = b - 1;
        if (!isPrimeGeneric((seq_t)b_minus_1)) return;
        ++count_pairs;
        return;
    }

    int p = fp_primes[idx];
    int e = fp_exps[idx];
    idx_t v = val;
    for (int c = 0; c <= 2 * e; ++c) {
        if (v > current_k) break;
        gen_divs(idx + 1, v);
        v = v * (idx_t)p;
    }
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* small primes are enough to factor early candidates */
        ensure_primes_up_to(100);
        initialized = true;
    }

    while (seq_len < n) {
        /* ensure prime list covers sqrt(next_k) */
        uint64_t bound = isqrt_uint64(next_k);
        if (bound < next_k) ++bound; /* strict upper bound */
        ensure_primes_up_to(bound);

        /* ----- factorisation of next_k ----- */
        idx_t t = next_k;
        int pcnt = 0;
        for (size_t i = 0; i < prime_cnt && (idx_t)prime_list[i] * (idx_t)prime_list[i] <= t; ++i) {
            int p = (int)prime_list[i];
            int cnt = 0;
            while (t % p == 0) {
                t /= p;
                ++cnt;
            }
            if (cnt > 0) {
                fp_primes[pcnt] = p;
                fp_exps[pcnt]   = cnt;
                ++pcnt;
            }
        }
        if (t > 1) {                                   /* remaining prime factor */
            fp_primes[pcnt] = (int)t;
            fp_exps[pcnt]   = 1;
            ++pcnt;
        }
        fp_cnt = pcnt;

        /* ----- test the unique factorisation condition ----- */
        current_k   = next_k;
        current_sq  = next_k * next_k;
        count_pairs = 0;

        gen_divs(0, 1);

        if (count_pairs == 1) {                         /* qualifies */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 32;
                seq = seq_realloc(seq, seq_cap, new_cap);
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)next_k;
        }

        ++next_k;
    }

    return seq[n - 1];
}
