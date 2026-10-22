/*
 * A328789  Expansion of (chi(x^3) / chi(-x^2))^2 in powers of x where chi()
 *          is a Ramanujan theta function.
 * https://oeis.org/A328789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 33, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS) A328789
 *  Formula (derived from the Euler product) :
 *      a(0) = 1
 *      for n ≥ 1:
 *          n * a(n) = Σ_{k=1}^{n} γ(k) * a(n-k)
 *      where γ(k) = 2 * Σ_{d | k, d ≡ 2,3,9,10 (mod 12)} d
 */

#include <stdlib.h>
#include <stdint.h>

/* dynamic cache for a(n) and γ(n) */
static seq_t *a_cache = NULL;          /* a(n) */
static seq_t *gamma   = NULL;          /* γ(n) */
static size_t cache_size = 0;          /* allocated size of both arrays */

/* residues for which γ contributes a divisor d */
static const int residues[4] = {2, 3, 9, 10};

/* extend cache to support indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_size)
        return;   /* already large enough */

    size_t new_size = (size_t)n + 1;

    /* resize arrays */
    a_cache  = (seq_t *)realloc(a_cache,  new_size * sizeof(seq_t));
    gamma    = (seq_t *)realloc(gamma,    new_size * sizeof(seq_t));

    /* initialise new slots to zero */
    for (size_t i = cache_size; i < new_size; ++i) {
        a_cache[i] = 0;
        gamma[i]   = 0;
    }

    /* initialise base case if this is the first allocation */
    if (cache_size == 0) {
        a_cache[0] = 1;   /* a(0) = 1 */
        gamma[0]   = 0;   /* unused */
    }

    /* recompute γ(k) for k = 1 … n */
    for (size_t i = 1; i <= (size_t)n; ++i)
        gamma[i] = 0;
    for (int r_idx = 0; r_idx < 4; ++r_idx) {
        int r = residues[r_idx];
        for (idx_t d = r; d <= n; d += 12) {
            seq_t contrib = (seq_t)(2 * d);   /* 2*d */
            for (idx_t m = d; m <= n; m += d) {
                gamma[m] += contrib;
            }
        }
    }

    /* compute a(m) for new indices (start at 1 to avoid division by 0) */
    for (size_t m = (cache_size == 0 ? 1 : cache_size); m < new_size; ++m) {
        seq_t sum = 0;
        for (size_t k = 1; k <= m; ++k) {
            if (gamma[k] != 0) {
                sum += gamma[k] * a_cache[m - k];
            }
        }
        a_cache[m] = sum / (seq_t)m;  /* division is exact by construction */
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    return a_cache[(size_t)n];
}
