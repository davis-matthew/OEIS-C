/*
 * A354351  Dirichlet inverse of A108951, primorial inflation of n.
 * https://oeis.org/A354351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 16, i32 n <= 28, i64 n <= 52, i128 n <= 100,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq: Dirichlet inverse of A108951 (primorial inflation)
   a(1) = 1
   a(n) is multiplicative
   a(p^e) = 0  if e > 1
   a(p)   = –primorial(p)   (primorial(p)= product of all primes ≤ p)
   Therefore for n = p1 p2 … pk  (distinct primes)
   a(n) = (–1)^k  · primorial(p1)  · primorial(p2) · … · primorial(pk)
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t limit, uint64_t **primes, size_t *cnt) */
#include "../utils/factor_u64.h"          /* factor_u64(uint64_t n, uint64_t *factors, uint64_t *exps, size_t *cnt) */

/* Cached data for prime list and their primorials */
static bool        init_done  = false;
static idx_t       prime_limit = 0;      /* highest number for which primes were generated */
static uint64_t   *prime_list  = NULL;   /* array of primes up to prime_limit */
static size_t       prime_cnt   = 0;   /* number of primes in prime_list     */
static seq_t      *primorial_arr;        /* primorial_arr[i] = product of primes[0..i] */

/* ------------------------------------------------------------------ */
/* Helper: binary search for the index of a prime in prime_list.      */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return mid;
        if (prime_list[mid] < p)  lo = mid + 1;
        else                      hi = mid;
    }
    /* p must be present (it comes from factor_u64), but guard anyway */
    return prime_cnt;
}

/* ------------------------------------------------------------------ */
/* Build or extend the prime list and primorial array up to "limit".  */
static void init_or_expand(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* free previous allocations, if any */
    if (prime_list) free(prime_list);
    if (primorial_arr) free(primorial_arr);

    /* generate all primes up to limit (inclusive) */
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;

    /* allocate primorial array */
    primorial_arr = (seq_t *)malloc(sizeof(seq_t) * prime_cnt);
    assert(primorial_arr != NULL);

    /* compute primorials: primorial_arr[i] = product of primes[0..i] */
    if (prime_cnt > 0) {
        primorial_arr[0] = (seq_t)prime_list[0];          /* 2# = 2 */
        for (size_t i = 1; i < prime_cnt; ++i) {
            primorial_arr[i] = primorial_arr[i - 1] * (seq_t)prime_list[i];
        }
    }

    init_done = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                                     */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;                /* a(1) = 1 */

    init_or_expand(n);                          /* ensure primes ≤ n are ready */

    /* factor n: obtain primes and exponents */
    uint64_t pf[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pf, ex, &cnt);

    /* if the factorization has any exponent > 1 -> result 0 */
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] > 1) return (seq_t)0;
    }

    /* accumulate the product of primorials, keep parity of count */
    seq_t prod = (seq_t)1;
    size_t odd = 0;     /* count of distinct primes */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pf[i];
        size_t idx = prime_index(p);
        prod *= primorial_arr[idx];
        odd ^= 1;           /* toggle parity */
    }

    return odd ? -prod : prod;
}
