/*
 * A344526  a(n) = Sum_{k=1..n} k^3 * phi(k).
 * https://oeis.org/A344526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^3 * phi(k)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 128, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9896 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Handle offset: for n==0 return 0 */
    if (n == 0) return (seq_t)0;

    /* Static caches */
    static seq_t *phi = NULL;      /* phi values, phi[0] unused */
    static seq_t *sum = NULL;      /* cumulative sums */
    static idx_t  max_computed = 0;   /* largest index computed so far */
    static size_t cache_cap = 0;      /* allocated size */

    /* Grow cache if needed */
    if (n >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? (n + 1) : (cache_cap * 2);
        if (new_cap <= n) new_cap = n + 1;
        seq_t *new_phi = mem_resize(seq_t, phi, new_cap);
        seq_t *new_sum = mem_resize(seq_t, sum, new_cap);
        if (!new_phi || !new_sum) {
            /* Allocation failed – abort quietly (could also use exit) */
            return (seq_t)0;
        }
        phi = new_phi;
        sum = new_sum;
        cache_cap = new_cap;
    }

    /* Compute phi for new indices and extend cumulative sum */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Compute φ(i) via naive factorisation */
        uint64_t m = i;
        seq_t result = (seq_t)i;
        for (uint64_t p = 2; p * p <= m; ++p) {
            if (m % p == 0) {
                while (m % p == 0) m /= p;
                result -= result / (seq_t)p;
            }
        }
        if (m > 1) {
            result -= result / (seq_t)m;
        }
        phi[i] = result;

        /* Compute k^3 * φ(k) and update cumulative sum */
        seq_t k3 = (seq_t)i * (seq_t)i * (seq_t)i;
        seq_t term = k3 * phi[i];
        sum[i] = sum[i - 1] + term;
    }
    max_computed = n;
    return sum[n];
}
