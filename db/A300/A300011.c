/*
 * A300011  Expansion of e.g.f. exp(Sum_{k>=1} phi(k)*x^k/k!), where phi() is
 *          the Euler totient function (A000010).
 * https://oeis.org/A300011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} phi(k) * binomial(n-1,k-1) * a(n-k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0) = 1; a(1) = 1; a(2) = 2; a(3) = 6; ...      */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cache for totient values (uint64_t) up to a given index. */
static uint64_t *phi_cache_u64 = NULL;
static idx_t phi_cache_u64_size = 0;

/* Cache for sequence values a(n). */
static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;

/* Ensure phi_cache_u64[0..N] is filled, using a simple sieve. */
static void ensure_phi_upto(idx_t N)
{
    if (N < 1) return;
    if (phi_cache_u64_size > N) return;

    uint64_t *new_cache = (uint64_t *)realloc(phi_cache_u64, (N + 1) * sizeof(uint64_t));
    if (!new_cache) abort();
    phi_cache_u64 = new_cache;

    for (idx_t i = 0; i <= N; ++i)
        phi_cache_u64[i] = i;

    for (idx_t p = 2; p <= N; ++p) {
        if (phi_cache_u64[p] == p) {            /* p is prime */
            for (idx_t j = p; j <= N; j += p) {
                phi_cache_u64[j] -= phi_cache_u64[j] / p;
            }
        }
    }
    phi_cache_u64_size = N;
}

/* Ensure a_cache[0..N] is computed. */
static void ensure_a_cache_upto(idx_t N)
{
    if (a_cache_size > N) return;

    seq_t *new_cache = (seq_t *)realloc(a_cache, (N + 1) * sizeof(seq_t));
    if (!new_cache) abort();
    a_cache = new_cache;

    /* Initialize a(0) = 1 if we are extending from scratch. */
    if (a_cache_size == 0)
        a_cache[0] = (seq_t)1;

    /* Compute a(n) for n = a_cache_size ... N. */
    for (idx_t n = a_cache_size; n <= N; ++n) {
        if (n == 0) continue;   /* already set */

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;      /* binomial(n-1,0) */

        for (idx_t k = 1; k <= n; ++k) {
            seq_t phi_k   = (seq_t)phi_cache_u64[k];
            seq_t term    = phi_k * binom * a_cache[n - k];
            sum += term;

            if (k < n) {
                seq_t num = (seq_t)(n - k);
                seq_t den = (seq_t)k;
                binom = binom * num / den;
            }
        }
        a_cache[n] = sum;
    }
    a_cache_size = N + 1;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)0;   /* guard for completeness */
    if (phi_cache_u64_size < n)   /* need phi up to n */
        ensure_phi_upto(n);
    if (a_cache_size   < n + 1)
        ensure_a_cache_upto(n);
    return a_cache[n];
}
