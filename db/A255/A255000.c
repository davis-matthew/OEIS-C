/*
 * A255000  Prime(n + d(n)), with d(n) = prime(n+1) - prime(n), for n >= 1.
 * https://oeis.org/A255000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n + prime(n+1) - prime(n))
 * Range:     u8 n <= 45, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Dynamic array of primes (1‑based indices). */
static uint64_t *prime_array = NULL;
static size_t prime_cnt      = 0;   /* number of primes stored */
static size_t prime_cap      = 0;   /* allocated capacity */

/* Append a new prime to the dynamic array. */
static void append_prime(uint64_t p)
{
    if (prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 128;
        /* realloc returns void* in C++ */
        void *tmp = realloc(prime_array, new_cap * sizeof(uint64_t));
        if (!tmp) abort();          /* allocation failure */
        prime_array = (uint64_t *)tmp;
        prime_cap   = new_cap;
    }
    prime_array[prime_cnt++] = p;
}

/* Integer square root of a 64‑bit number using binary search. */
static uint64_t isqrt_uint64(uint64_t v)
{
    if (v < 2) return v;
    uint64_t lo = 1, hi = v / 2, mid, res = 1;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        uint64_t sq = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            res = mid;
            lo  = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

/* Test whether a candidate is prime using trial division by stored primes. */
static bool is_prime_candidate(uint64_t cand)
{
    if (cand < 2)               return false;
    if (cand == 2)              return true;
    if ((cand & 1) == 0)        return false;

    uint64_t limit = isqrt_uint64(cand);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_array[i];
        if (p > limit) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* Ensure that at least the first `idx` primes are generated. */
static void ensure_primes(idx_t idx)
{
    if (prime_cnt >= (size_t)idx) return;

    /* Start from the next candidate after the last prime. */
    uint64_t cand = prime_cnt ? prime_array[prime_cnt - 1] + 2 : 2;
    if (cand == 2) {
        /* Special case for 2 */
        append_prime(2);
        ++cand;  /* next candidate will be 3 */
    }
    while (prime_cnt < (size_t)idx) {
        if (is_prime_candidate(cand)) {
            append_prime(cand);
        }
        cand += 2; /* skip even numbers */
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined below offset, but guard */

    /* Need prime(n) and prime(n+1). */
    ensure_primes(n + 1);

    uint64_t pn  = prime_array[n - 1];
    uint64_t pnp1 = prime_array[n];
    uint64_t d   = pnp1 - pn;            /* gap */
    idx_t target_idx = n + (idx_t)d;      /* n + d(n) */

    /* Ensure the prime at the target index exists. */
    ensure_primes(target_idx);

    return (seq_t)prime_array[target_idx - 1];
}
