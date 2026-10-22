/*
 * A366383  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A349915(i) = A349915(j) for all i, j >= 1, where A349915 is
 *          Dirichlet inverse of arithmetic mean between the number of odd
 *          divisors and their sum.
 * https://oeis.org/A366383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   lexicographically earliest a(n) with a(i)=a(j) ↔
 *            A349915(i)=A349915(j), where A349915 is the Dirichlet inverse of
 *            h(n) = (τodd(n)+σodd(n))/2
 * Range:     u8 n <= 242, u16 n <= 9861, u32 n <= 9861, u64 n <= 9861,
 *            u128 n <= 9861, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Global caches */
static seq_t *tau_cache   = NULL;
static seq_t *sigma_cache = NULL;
static seq_t *h_cache     = NULL;
static seq_t *inv_cache   = NULL;
static seq_t *a_cache     = NULL;
static size_t cache_size  = 0;   /* current maximum index cached */

/* Return odd part of n (remove all factors of 2). */
static inline uint64_t odd_part_uint64(uint64_t n)
{
    while ((n & 1ULL) == 0ULL) n >>= 1;
    return n;
}

/* Ensure all arrays are computed up to N (inclusive). */
static void ensure_cache(idx_t N)
{
    if ((idx_t)(N + 1) <= cache_size) return;   /* already computed */

    size_t sz = (size_t)(N + 1);                 /* array size (0..N) */
    seq_t  *tau   = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t  *sigma = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t  *h     = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t  *inv   = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t  *a     = (seq_t*)calloc(sz, sizeof(seq_t));

    /* Sieve for divisor count (τ) and divisor sum (σ). */
    for (size_t d = 1; d < sz; ++d) {
        seq_t dseq = (seq_t)d;
        for (size_t m = d; m < sz; m += d) {
            ++tau[m];
            sigma[m] += dseq;
        }
    }

    /* h[n] = (τodd(n)+σodd(n))/2, where odd part of n is odd. */
    for (size_t n = 1; n < sz; ++n) {
        uint64_t odd = odd_part_uint64((uint64_t)n);
        h[n] = (tau[odd] + sigma[odd]) >> 1;   /* divide by 2 */
    }

    /* Dirichlet inverse of h */
    inv[1] = (seq_t)1;
    for (size_t n = 2; n < sz; ++n) {
        seq_t sum = (seq_t)0;
        /* Loop over all divisors d of n with d>1 */
        for (size_t d = 2; d * d <= n; ++d) {
            if (n % d == 0) {
                size_t e = n / d;
                sum += h[d] * inv[e];
                if (e != d && e > 1) {
                    sum += h[e] * inv[d];
                }
            }
        }
        /* contribution from divisor d = n, e = 1 */
        sum += h[n] * inv[1];
        inv[n] = -sum;
    }

    /* Build the lexicographically earliest sequence a(n). */
    seq_t next_val = (seq_t)1;
    a[1] = next_val;
    ++next_val;
    for (size_t n = 2; n < sz; ++n) {
        seq_t val = (seq_t)0;      /* 0 marks “not yet assigned” */
        for (size_t j = 1; j < n; ++j) {
            if (inv[j] == inv[n]) {   /* same A349915 value */
                val = a[j];
                break;
            }
        }
        if (val == (seq_t)0) {   /* first occurrence of this inverse value */
            val = next_val;
            ++next_val;
        }
        a[n] = val;
    }

    /* Replace old caches with new ones */
    free(tau_cache);
    free(sigma_cache);
    free(h_cache);
    free(inv_cache);
    free(a_cache);
    tau_cache   = tau;
    sigma_cache = sigma;
    h_cache     = h;
    inv_cache   = inv;
    a_cache     = a;
    cache_size  = sz - 1;   /* maximum index stored */
}

/* Public function returning the n‑th term of A366383 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n=0 */
    ensure_cache(n);
    return a_cache[(size_t)n];
}
