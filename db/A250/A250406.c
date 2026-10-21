/*
 * A250406  Values of B such that p = prime(n) satisfies (p-1)! == -1-B*p (mod
 *          p^2), i.e., p is a near-Wilson prime.
 * https://oeis.org/A250406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B where p = prime(n) satisfies (p-1)! ≡ -1 - B·p (mod p²)
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A250406.  */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto */

static uint64_t *prime_arr   = NULL;   /* array of primes (1‑based indexing in a) */
static size_t   prime_cnt    = 0;      /* number of primes currently stored */

/* Estimate an upper bound for the n-th prime (n >= 1).  */
static idx_t estimate_prime_limit(idx_t n)
{
    if (n < 6) return 20;                  /* p_6 = 13, 20 is safe */
    double dn   = (double)n;
    double ub   = dn * (log(dn) + log(log(dn))) + 3.0;   /* +3 for safety   */
    return (idx_t)ub;
}

/* Ensure prime_arr contains at least n primes (prime_arr[0] … prime_arr[n‑1]) */
static void ensure_primes(idx_t n)
{
    if (n <= prime_cnt) return;           /* already have enough primes */

    idx_t limit = estimate_prime_limit(n);
    uint64_t *new_arr = NULL;
    size_t new_cnt = 0;

    /* build_primes_upto allocates the array and sets new_arr/new_cnt */
    build_primes_upto(limit, &new_arr, &new_cnt);

    /* Free the old array if any */
    free(prime_arr);

    prime_arr  = new_arr;
    prime_cnt  = new_cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety for offset 1 */

    /* Obtain the n-th prime (1‑based) */
    ensure_primes(n);
    uint64_t p = prime_arr[n - 1];       /* prime[n]  */

    /* Compute (p-1)! modulo p² */
    __uint128_t mod   = (__uint128_t)p * p;
    __uint128_t fact  = 1;
    for (uint64_t i = 2; i < p; ++i) {
        fact = (fact * i) % mod;
    }

    /* sum = fact + 1 (mod p²) */
    __uint128_t sum = fact + 1;
    if (sum >= mod) sum -= mod;          /* now sum < p² */

    /* k = sum / p  (integer division, 0 ≤ k < p) */
    uint64_t k = sum / p;

    /* B ≡ -k (mod p), we take the non‑negative residue 0 … p-1 */
    uint64_t B = (p - k) % p;

    return (seq_t)B;
}
