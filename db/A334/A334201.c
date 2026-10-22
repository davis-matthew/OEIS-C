/*
 * A334201  a(n) = A056239(n) - A061395(n).
 * https://oeis.org/A334201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A056239(n) - A061395(n) where A056239(n) = Σ(p_i
 *            index)·e_i for each prime factor p_i^e_i of n and A061395(n) =
 *            max(p_i index) (1‑based index of the largest prime factor). a(1)
 *            = 0
 * Range:     u8 n <= 9943, u16 n <= 9943, u32 n <= 9943, u64 n <= 9943,
 *            u128 n <= 9943, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list_u64 = NULL;  /* primes up to current limit */
static size_t   prime_cnt  = 0;           /* number of primes in prime_list_u64 */
static idx_t    prime_limit = 0;          /* current maximum constructed value */

/* Return the 0‑based index of prime p in prime_list_u64 (assumes p is prime). */
static int
prime_index(idx_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list_u64[mid] <= (uint64_t)p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (int)(lo - 1);   /* lo stopped at first element > p */
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* a(1) = 0 */

    /* Ensure prime list contains all primes up to n. */
    if (prime_limit < n) {
        free(prime_list_u64);          /* safe: free(NULL) is no‑op */
        prime_list_u64 = NULL;
        build_primes_upto(n, &prime_list_u64, &prime_cnt);
        prime_limit = n;
    }

    /* Factor n. */
    static uint64_t pf[64];
    static uint64_t exp[64];
    size_t fcnt = 0;
    factor_u64((uint64_t)n, pf, exp, &fcnt);

    seq_t sum_parts = 0;
    seq_t max_k    = 0;

    for (size_t i = 0; i < fcnt; ++i) {
        idx_t  p   = (idx_t)pf[i];
        idx_t  e   = (idx_t)exp[i];
        int    idx = prime_index(p);          /* 0‑based index of prime p */
        seq_t  k   = (seq_t)(idx + 1);        /* 1‑based index */

        sum_parts += k * (seq_t)e;
        if (k > max_k) max_k = k;
    }

    return sum_parts - max_k;
}
