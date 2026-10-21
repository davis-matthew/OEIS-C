/*
 * A259279  a(n) = Sum_{k=0..n} k^2 * A000041(k).
 * https://oeis.org/A259279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} k^2 * p(k) (p(k) = A000041(k))
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_seq.h"

/* External types supplied by the build */
extern seq_t a(idx_t n); /* will be defined below */

/* Utility for dynamic allocation of seq_t arrays (pointer to arbitrary ‑precision int) */

static seq_t *realloc_seq(seq_t *p, size_t n)
{
    return (seq_t *)realloc(p, n * sizeof(seq_t));
}
static void free_seq_arr(seq_t *p)
{
    free(p);
}

/* Partition numbers p(k)   □ = A000041(k)   */
static seq_t *p_cache = NULL;          /* cached partition numbers */
static idx_t  p_cap   = 0;            /* capacity of p_cache      */
static idx_t  p_nmax  = 0;            /* greatest index computed */

/* Ensure that p_cache[n] is valid for all 0 <= n <= N */
static void compute_partitions_up_to(idx_t N)
{
    /* Allocate or grow the cache to hold indices 0..N */
    if (p_cache == NULL) {
        p_cache = alloc_seq((size_t)(N + 1));
        p_cap   = N + 1;
        p_nmax  = 0;
        /* p(0) = 1 */
        p_cache[0] = 1;
    } else if (p_cap <= N) {
        p_cache = realloc_seq(p_cache, (size_t)(N + 1));
        p_cap   = N + 1;
    }

    /* Compute any new partition numbers beyond the current maximum */
    for (idx_t n = p_nmax + 1; n <= N; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            /* Generalised pentagonal numbers g1 = k(3k-1)/2 , g2 = k(3k+1)/2 */
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n) break;

            if (k & 1)
                sum += p_cache[n - g1];
            else
                sum -= p_cache[n - g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (n >= g2) {
                if (k & 1)
                    sum += p_cache[n - g2];
                else
                    sum -= p_cache[n - g2];
            }
        }
        p_cache[n] = sum;
    }
    p_nmax = N;
}

/* Desired sequence a(n) */
static seq_t *a_cache = NULL;          /* cached sequence values    */
static idx_t  a_cap   = 0;             /* capacity of a_cache      */
static idx_t  a_nmax  = 0;             /* greatest index computed */

/* Ensure that a_cache[n] is valid for all 0 <= n <= N */
static void compute_a_up_to(idx_t N)
{
    /* Allocate or grow the cache to hold indices 0..N */
    if (a_cache == NULL) {
        a_cache = alloc_seq((size_t)(N + 1));
        a_cap   = N + 1;
        a_nmax  = 0;
        a_cache[0] = 0;                /* a(0) = 0 */
    } else if (a_cap <= N) {
        a_cache = realloc_seq(a_cache, (size_t)(N + 1));
        a_cap   = N + 1;
    }

    /* Make sure partition numbers up to N are available */
    if (p_nmax < N)
        compute_partitions_up_to(N);

    /* Compute any new a(n) values beyond the current maximum */
    for (idx_t n = a_nmax + 1; n <= N; ++n) {
        seq_t sn   = (seq_t)n;
        seq_t term = sn * sn * p_cache[n];
        a_cache[n] = a_cache[n - 1] + term;
    }
    a_nmax = N;
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (a_nmax < n)
        compute_a_up_to(n);
    return a_cache[n];
}
