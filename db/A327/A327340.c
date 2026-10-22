/*
 * A327340  Numerator of the rationals r(n) = (1/n^2)*Phi_1(n), with Phi_1(n)
 *          = Sum{k=1..n} psi(k), with Dedekind's psi function.
 * https://oeis.org/A327340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements numerator of r(n) = (1/n^2)*∑_{k=1}^n ψ(k).
   ψ(k) = k * ∏_{p|k}(1 + 1/p) is computed for all k up to the
   largest requested n using a sieve.
   The numerator is returned as a(seq_t) after reducing the
   fraction S(n)/n^2 by gcd(S(n), n^2). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

typedef uint64_t idx_t;  /* placeholder; actual type is supplied by the build */

/* static cache */
static idx_t max_cached = 0;
static seq_t *psi_cache = NULL;    /* ψ(k) for k <= max_cached */
static seq_t *prefix_cache = NULL; /* prefix sums of ψ(k) */

/* recompute psi and prefix sums up to N (inclusive) */
static void recompute_upto(idx_t N)
{
    /* allocate new arrays */
    seq_t *new_psi = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *new_prefix = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!new_psi || !new_prefix) {
        abort(); /* memory allocation failed */
    }

    /* initialise ψ(k) = k */
    for (idx_t i = 1; i <= N; ++i) {
        new_psi[i] = (seq_t)i;
    }

    /* simple sieve to get primes up to N */
    bool *is_comp = (bool*)calloc((size_t)(N + 1), sizeof(bool));
    if (!is_comp) abort();
    idx_t *primes = NULL; size_t primes_cap = 0, primes_cnt = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            /* store prime */
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (idx_t*)realloc(primes, primes_cap * sizeof(idx_t));
                if (!primes) abort();
            }
            primes[primes_cnt++] = i;
            /* mark multiples */
            if (i * i <= N) {
                for (idx_t j = i * i; j <= N; j += i) {
                    is_comp[j] = true;
                }
            }
        }
    }
    free(is_comp);

    /* compute ψ using the sieve */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        idx_t p = primes[pi];
        seq_t p_seq = (seq_t)p;
        for (idx_t m = p; m <= N; m += p) {
            /* ψ(m) = ψ(m)/p * (p+1) */
            new_psi[m] = (new_psi[m] / p_seq) * (p_seq + (seq_t)1);
        }
    }
    free(primes);

    /* prefix sums */
    new_prefix[0] = 0;
    for (idx_t i = 1; i <= N; ++i) {
        new_prefix[i] = new_prefix[i - 1] + new_psi[i];
    }

    /* replace old caches */
    free(psi_cache);
    free(prefix_cache);
    psi_cache = new_psi;
    prefix_cache = new_prefix;
    max_cached = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe guard */
    if (n > max_cached) {
        recompute_upto(n);
    }
    seq_t S = prefix_cache[n];
    seq_t denom = (seq_t)n * (seq_t)n;
    seq_t g = gcd(S, denom);
    return S / g;
}
