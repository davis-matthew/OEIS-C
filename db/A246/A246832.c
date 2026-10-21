/*
 * A246832  Expansion of psi(x) * psi(x^2) * phi(x^2) in powers of x where
 *          phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A246832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions (a,b,c) with a,b≥0, c∈ℤ such that *
 *            T_a + b(b+1) + 2c² = n, where T_a = a(a+1)/2. * Solutions with
 *            c≠0 are counted twice (for ±c)
 * Range:     u8 n <= 379, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for sequence A246832. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Resize or compute cache if needed. */
    if (n >= cache_size)
    {
        /* Allocate new cache. */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)calloc((size_t)new_size, sizeof(seq_t));
        if (new_cache == NULL)
            return (seq_t)0;  /* allocation failed; return 0 as fallback */

        /* Precompute values up to n. */
        seq_t n_seq = (seq_t)n;
        for (idx_t a_idx = 0;; ++a_idx)
        {
            seq_t Ta = (seq_t)a_idx * ((seq_t)a_idx + 1) / 2;
            if (Ta > n_seq)
                break;
            for (idx_t b_idx = 0;; ++b_idx)
            {
                seq_t Tb = (seq_t)b_idx * ((seq_t)b_idx + 1);
                seq_t ab = Ta + Tb;
                if (ab > n_seq)
                    break;
                seq_t remaining = n_seq - ab;
                for (idx_t c_idx = 0;; ++c_idx)
                {
                    seq_t sum = ab + (seq_t)2 * c_idx * c_idx;
                    if (sum > n_seq)
                        break;
                    seq_t weight = (c_idx == 0) ? 1 : 2;
                    new_cache[(size_t)sum] += weight;
                }
            }
        }

        /* Replace old cache. */
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
