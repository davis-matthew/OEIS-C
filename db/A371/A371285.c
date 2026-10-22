/*
 * A371285  Heinz number of the multiset union of the divisor sets of each
 *          prime index of n.
 * https://oeis.org/A371285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} A275700(index(p))^e where A275700(k) = ∏_{d|k}
 *            prime(d) and prime(d) is the d‑th prime (1‑based)
 * Range:     u8 n <= 12, u16 n <= 36, u32 n <= 150, u64 n <= 658,
 *            u128 n <= 2422, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes.h"

static idx_t *primes_arr   = NULL;   /* all primes up to a fixed upper bound  */
static size_t primes_cnt   = 0;      /* number of primes in primes_arr      */
static seq_t *A275_arr     = NULL;   /* cache for A275700 values            */
static size_t A275_size    = 0;      /* size of A275_arr (A275_size-1 = max index) */

/* Build all primes up to LIMIT (inclusive) and fill primes_arr and primes_cnt.
   Then compute A275700(k) for k = 1 .. primes_cnt using a sieve‑style method.  */
static void init_global(void)
{
    if (primes_arr != NULL) return;   /* already initialized */

    const idx_t LIMIT = 200000;        /* sufficient for n ≤ 10000                */
    build_primes(LIMIT, &primes_arr, &primes_cnt);

    /* Allocate and compute A275700 values.  */
    A275_size = primes_cnt + 1;                /* index 0 unused, indices 1..primes_cnt */
    A275_arr  = (seq_t *)malloc(A275_size * sizeof(seq_t));
    if (A275_arr == NULL) {                    /* OOM: abort silently                 */
        exit(1);
    }
    /* Initialize all entries to 1. */
    for (idx_t k = 1; k < A275_size; ++k) A275_arr[k] = (seq_t)1;

    /* For each divisor d, multiply prime(d) into all multiples of d. */
    for (idx_t d = 1; d <= primes_cnt; ++d) {
        seq_t prime_d = primes_arr[d - 1];
        for (idx_t m = d; m <= primes_cnt; m += d) {
            A275_arr[m] *= prime_d;
        }
    }
}

/* Binary search for the 1‑based index of prime p in primes_arr.  */
static idx_t prime_index(uint64_t p)
{
    idx_t lo = 1, hi = (idx_t)primes_cnt;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (primes_arr[mid - 1] == p)
            return mid;
        if (primes_arr[mid - 1] < p)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return 0;   /* should never happen for primes dividing n */
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if (primes_arr == NULL)
        init_global();

    seq_t res = (seq_t)1;
    uint64_t x   = (uint64_t)n;

    /* Factor n using the precomputed primes list.  */
    for (idx_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes_arr[i];
        if (p > x / p)    /* p^2 > x, stop. */
            break;
        if (x % p == 0) {
            idx_t idx = i + 1;                /* 1‑based index of p           */
            seq_t val = A275_arr[idx];
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            for (int k = 0; k < e; ++k)
                res *= val;
        }
    }

    /* Remaining prime factor, if any.  */
    if (x > 1) {
        idx_t idx = prime_index(x);
        seq_t val = A275_arr[idx];
        res *= val;
    }

    return res;
}
