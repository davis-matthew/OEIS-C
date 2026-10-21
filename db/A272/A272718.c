/*
 * A272718  Partial sums of gcd-sum sequence A018804.
 * https://oeis.org/A272718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( n*(n+1)/2 + Σ_{k=1..n} φ(k)·⌊n/k⌋² ) / 2
 * Range:     u8 n <= 10, u16 n <= 132, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static cache for Euler totient values (uint64_t) up to the largest n seen */
static uint64_t *phi_cache = NULL;
static size_t phi_cap = 0;

/* recompute Euler totients up to n (inclusive) */
static void ensure_phi_cache(idx_t n)
{
    if (phi_cap > (size_t)n) return;          /* already computed up to n */

    size_t new_cap = (size_t)n + 1;
    uint64_t *new_cache = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
    if (!new_cache) return;                  /* allocation failure – abort silently */

    /* initialise phi[i] = i */
    for (size_t i = 1; i <= (size_t)n; ++i)
        new_cache[i] = (uint64_t)i;

    /* sieve for primes up to n */
    bool *is_prime = (bool *)malloc(new_cap * sizeof(bool));
    if (!is_prime) { free(new_cache); return; }
    for (size_t i = 0; i < new_cap; ++i) is_prime[i] = true;
    if (new_cap > 0) is_prime[0] = false;
    if (new_cap > 1) is_prime[1] = false;
    for (size_t p = 2; p * p <= (size_t)n; ++p) {
        if (is_prime[p]) {
            for (size_t j = p * p; j <= (size_t)n; j += p)
                is_prime[j] = false;
        }
    }

    /* apply totient reduction: φ[j] -= φ[j] / p for each prime p */
    for (size_t p = 2; p <= (size_t)n; ++p) {
        if (is_prime[p]) {
            for (size_t j = p; j <= (size_t)n; j += p) {
                new_cache[j] -= new_cache[j] / (uint64_t)p;
            }
        }
    }

    free(is_prime);
    if (phi_cache) free(phi_cache);
    phi_cache = new_cache;
    phi_cap   = new_cap - 1;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not defined, but safe */

    ensure_phi_cache(n);

    seq_t n_seq = (seq_t)n;
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t t = n_seq / (seq_t)k;              /* floor(n/k) */
        seq_t phi_k = (seq_t)phi_cache[(size_t)k];
        sum += phi_k * t * t;
    }

    seq_t total = n_seq * (n_seq + (seq_t)1) / (seq_t)2 + sum;
    return total / (seq_t)2;                    /* exact division by 2 */
}
