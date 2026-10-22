/*
 * A363620  Reverse-weighted alternating sum of the multiset of prime indices
 *          of n.
 * https://oeis.org/A363620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A363620:
 *  a(n) = reverse‑weighted alternating sum of the multiset of prime indices of n.
 *  The multiset is obtained by factorising n and writing the 1‑based index of
 *  each prime factor (repeated for repeated factors).  The reverse‑weighted
 *  alternating sum of a sequence (y1,…,yk) is Σ i=1..k (-1)^(k-i) i * y_{k-i+1}.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/compute_pi.h"   /* seq_t compute_pi(idx_t) */

/* Integer square root: floor(sqrt(n)) */
static idx_t int_sqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t left = 1, right = n >> 1, mid, res = 0;
    while (left <= right) {
        mid = left + ((right - left) >> 1);
        if (mid <= n / mid) {
            res = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return res;
}

/* Cache for prime numbers up to the current limit */
static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Build a list of primes up to 'limit' (inclusive).  The table is
 * rebuilt only when a larger limit is requested.  Memory is allocated
 * with malloc/realloc and explicitly cast for C++ compatibility. */
static void sieve_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;               /* already up to date */

    /* Allocate a boolean table for composites.  Use malloc and cast. */
    uint8_t *comp = (uint8_t *)calloc((size_t)(limit + 1), 1);
    if (!comp) exit(1);

    /* Allocate or reallocate storage for prime numbers. */
    idx_t *new_list = (idx_t *)realloc(prime_list, (size_t)(limit + 1) * sizeof(idx_t));
    if (!new_list) { free(comp); exit(1); }
    prime_list = new_list;

    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            prime_list[prime_count++] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    comp[j] = 1;
            }
        }
    }
    free(comp);
    prime_limit = limit;
}

/* Compute the reverse‑weighted alternating sum for a single n. */
static seq_t reverse_weighted_sum(idx_t n)
{
    if (n <= 1) return 0;            /* empty multiset for 1 or 0 */

    /* Factorise n.  We need primes up to sqrt(n). */
    idx_t limit = int_sqrt(n);
    if (limit < 2) limit = 2;
    sieve_primes_upto(limit);

    idx_t remaining = n;
    /* Storage for indices; 256 is more than enough for typical n. */
    seq_t indices[256];
    size_t k = 0;

    /* Trial division using the stored primes. */
    for (size_t pi = 0; pi < prime_count && prime_list[pi] * prime_list[pi] <= remaining; ++pi) {
        idx_t p = prime_list[pi];
        if (remaining % p == 0) {
            seq_t prime_idx = (seq_t)(pi + 1);   /* 1‑based index of p */
            while (remaining % p == 0) {
                remaining /= p;
                if (k < sizeof(indices) / sizeof(indices[0]))
                    indices[k++] = prime_idx;
                else
                    return 0;   /* Should never happen for reasonable n */
            }
        }
    }

    /* If a prime > sqrt(n) remains, it appears once. */
    if (remaining > 1) {
        /* compute_pi(remaining) gives the index of that prime. */
        seq_t prime_idx = compute_pi(remaining);
        if (k < sizeof(indices) / sizeof(indices[0]))
            indices[k++] = prime_idx;
        else
            return 0;
    }

    /* Reverse‑weighted alternating sum: Σ (-1)^(k-i-1) * (i+1) * indices[k-i-1] */
    seq_t sum = 0;
    for (size_t i = 0; i < k; ++i) {
        seq_t sign = (((k - 1 - i) & 1) ? -1 : 1);
        sum += sign * (seq_t)(i + 1) * indices[k - 1 - i];
    }
    return sum;
}

/* Public sequence function.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* sequence undefined below offset */
    return reverse_weighted_sum(n);
}
