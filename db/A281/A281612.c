/*
 * A281612  Expansion of Sum_{i = p*q, p prime, q prime} x^i/(1 - x^i) /
 *          Product_{j>=1} (1 - x^j).
 * https://oeis.org/A281612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   free_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A281612 – total number of semiprime parts in all partitions of n
    a(n) =Sum_{i=p*q}  Sum_{k>=1} p(n-i*k)
    where p(m) is the partition function and the inner sum counts
    partitions with at least one part equal to a multiple of a semiprime.
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/free_arr.h"

/* global state for memoisation */
static idx_t max_n = 0;     /* largest n already computed */
static seq_t *p    = NULL;  /* partition numbers p[0..max_n]          */
static seq_t *c    = NULL;  /* c[d] = number of semiprime divisors of d  */
static seq_t *a_val= NULL;  /* a(n) values for 0..max_n               */

/* allocate an array of seq_t of length N, zero‑initialised */
static seq_t *alloc_seq_arr(size_t N)
{
#ifdef __cplusplus
    return new seq_t[N]();               /* value‑initialise (zero) */
#else
    return (seq_t*)calloc(N, sizeof(seq_t));
#endif
}

/* compute all tables up to n (inclusive) */
static void compute_up_to(idx_t n)
{
    size_t N = (size_t)n + 1;          /* number of elements per array */

    /* free old tables if any */
    free_arr(p);
    free_arr(c);
    free_arr(a_val);

    /* allocate new arrays, zero‑initialised */
    p     = alloc_seq_arr(N);
    c     = alloc_seq_arr(N);
    a_val = alloc_seq_arr(N);
    if (!p || !c || !a_val) exit(1);   /* allocation failed */

    /* ---------- 1. Partition numbers p[0..N-1] ------------------------ */
    p[0] = 1;                           /* p(0)=1 */
    for (size_t m = 1; m < N; ++m)
        for (size_t k = m; k < N; ++k)
            p[k] += p[k - m];

    /* ---------- 2. Prime sieve up to N-1 ------------------------------ */
    bool *is_prime = (bool*)calloc(N, sizeof(bool));
    if (!is_prime) exit(1);
    if (N > 2) {
        for (size_t i = 2; i < N; ++i) is_prime[i] = true;
        for (size_t i = 2; i * i < N; ++i)
            if (is_prime[i])
                for (size_t j = i * i; j < N; j += i)
                    is_prime[j] = false;
    }

    /* build list of primes up to N-1 */
    size_t prime_cnt = 0;
    for (size_t i = 2; i < N; ++i)
        if (is_prime[i]) ++prime_cnt;

    size_t *primes = (size_t*)malloc(prime_cnt * sizeof(size_t));
    if (!primes) exit(1);
    for (size_t i = 2, j = 0; i < N; ++i)
        if (is_prime[i]) primes[j++] = i;

    /* ---------- 3. Semiprime flag ------------------------------------- */
    bool *is_semiprime = (bool*)calloc(N, sizeof(bool));
    if (!is_semiprime) exit(1);
    for (size_t i = 0; i < prime_cnt; ++i) {
        size_t p1 = primes[i];
        for (size_t j = i; j < prime_cnt; ++j) {
            size_t p2 = primes[j];
            size_t prod = p1 * p2;
            if (prod >= N) break;
            is_semiprime[prod] = true;
        }
    }

    /* ---------- 4. c[d] = # of semiprime divisors of d ---------------- */
    for (size_t d = 2; d < N; ++d)
        if (is_semiprime[d])
            for (size_t m = d; m < N; m += d)
                ++c[m];

    /* ---------- 5. Convolution a[n] = sum_{k=0}^n p[k]*c[n-k] --------- */
    for (size_t k = 0; k < N; ++k) {
        seq_t pk = p[k];
        for (size_t n2 = k; n2 < N; ++n2)
            a_val[n2] += pk * c[n2 - k];
    }

    /* free temporaries */
    free(is_prime);
    free(primes);
    free(is_semiprime);

    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1, a(0) unused */
    if (n > max_n) compute_up_to(n);
    return a_val[n];
}
