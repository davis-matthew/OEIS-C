/*
 * A397820  The positive integers ordered by decreasing values of n ->
 *          prime(n+1)/prime(n).
 * https://oeis.org/A397820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 9372, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A397820: permutation of natural numbers ordered by decreasing
 * ratio p_{n+1}/p_n  (p_k is the k-th prime) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* -------------------------------------------------------------
 *  Static data for primality, permutation and cache.
 * ------------------------------------------------------------- */
static uint64_t *prime_list      = NULL;  /* primes[0] = p1 = 2                */
static idx_t    prime_count      = 0;     /* number of primes stored           */
static idx_t  *perm              = NULL;  /* first 10 000 terms of the sequence*/
static idx_t    perm_size        = 0;     /* number of terms computed          */

/* -------------------------------------------------------------
 *  Parameters
 * ------------------------------------------------------------- */
#define TOP_TERMS        10000          /* number of permutation terms needed */
#define TOP_INDICES      1000000        /* number of indices to consider      */
#define SIEVE_UPPER      20000000       /* sieve up to this value (≈20·10⁶)    */

/* -------------------------------------------------------------
 *  Helper: builds prime list up to at least TOP_INDICES+1 primes
 * ------------------------------------------------------------- */
static void build_primes(void)
{
    if (prime_list != NULL) return;        /* already built */

    /* Allocate storage for primes. We need at least TOP_INDICES+1. */
    prime_count = 0;
    prime_list  = (uint64_t *)malloc((TOP_INDICES + 2) * sizeof(uint64_t));
    if (!prime_list) abort();

    /* Sieve of Eratosthenes up to SIEVE_UPPER. Stop once enough
     * primes are collected. */
    bool *comp = (bool *)calloc(SIEVE_UPPER + 1, sizeof(bool));
    if (!comp) abort();

    for (size_t i = 2; i <= SIEVE_UPPER && prime_count < (TOP_INDICES + 1); ++i)
    {
        if (!comp[i])
        {
            if (prime_count < (TOP_INDICES + 1))
                prime_list[prime_count++] = (uint64_t)i;

            if (i * i <= SIEVE_UPPER)   /* avoid overflow */
                for (size_t j = i * i; j <= SIEVE_UPPER; j += i)
                    comp[j] = true;
        }
    }

    free(comp);
}

/* -------------------------------------------------------------
 *  Comparator for qsort: order indices by decreasing
 *  p_{n+1}/p_n ratio.
 * ------------------------------------------------------------- */
static int cmp_ratio(const void *a, const void *b)
{
    idx_t i = *(const idx_t *)a;
    idx_t j = *(const idx_t *)b;

    /* ratio for index k is p_{k+1}/p_k   => primes[k] / primes[k-1]   */
    __int128 lhs = (__int128)prime_list[i]     * prime_list[j - 1];
    __int128 rhs = (__int128)prime_list[j]     * prime_list[i - 1];

    if (lhs > rhs) return -1;  /* descending */
    if (lhs < rhs) return  1;
    return 0;
}

/* -------------------------------------------------------------
 *  Builds the first TOP_TERMS of the permutation.
 * ------------------------------------------------------------- */
static void build_permutation(void)
{
    if (perm_size == TOP_TERMS) return; /* already built */

    build_primes();                     /* ensure primes ready */

    /* Create list of indices 1..TOP_INDICES */
    idx_t *indices = (idx_t *)malloc(TOP_INDICES * sizeof(idx_t));
    if (!indices) abort();
    for (idx_t i = 1; i <= TOP_INDICES; ++i)
        indices[i - 1] = i;

    /* Sort indices by decreasing p_{n+1}/p_n */
    qsort(indices, TOP_INDICES, sizeof(idx_t), cmp_ratio);

    /* Keep only the first TOP_TERMS ranks */
    perm = (idx_t *)malloc(TOP_TERMS * sizeof(idx_t));
    if (!perm) abort();
    for (idx_t i = 0; i < TOP_TERMS; ++i)
        perm[i] = indices[i];
    perm_size = TOP_TERMS;

    free(indices);
}

/* -------------------------------------------------------------
 *  Public sequence function
 * ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n==0 */

    build_permutation();                /* ensure data ready */

    if (n > perm_size)                  /* out of precomputed range
                                         * return 0 (undefined but safe) */
        return 0;

    /* Cast to the build‑time defined seq_t type */
    return (seq_t)perm[n - 1];
}
