/*
 * A304824  Convolution of central binomial coefficients and partition
 *          numbers.
 * https://oeis.org/A304824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} C(2*k,k) * p(n-k) where C(2*k,k) are central
 *            binomial coefficients and p(m) are partition numbers
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *p_cache = NULL;      /* partition numbers */
static size_t p_cap = 0;
static seq_t *c_cache = NULL;      /* C(2*k,k) */
static size_t c_cap = 0;

/* Ensure the cache big enough for indices up to n */
static void ensure_size(idx_t n)
{
    if (n >= p_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_p = alloc_seq_array(new_cap);
        for (size_t i = 0; i < p_cap; ++i) new_p[i] = p_cache[i];
        /* compute missing partition numbers */
        if (p_cap == 0) {
            new_p[0] = 1;
            p_cap = 1;
        }
        for (idx_t i = (idx_t)p_cap; i <= n; ++i) {
            seq_t pos = 0, neg = 0;
            for (idx_t k = 1; ; ++k) {
                idx_t g1 = k * (3 * k - 1) / 2;
                if (g1 > i) break;
                if ((k & 1) != 0) pos += new_p[i - g1];
                else              neg += new_p[i - g1];

                idx_t g2 = k * (3 * k + 1) / 2;
                if (g2 > i) continue;
                if ((k & 1) != 0) pos += new_p[i - g2];
                else              neg += new_p[i - g2];
            }
            new_p[i] = pos - neg;
        }
        if (p_cache != NULL) free_seq_array(p_cache);
        p_cache = new_p;
        p_cap = new_cap;
    }

    if (n >= c_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_c = alloc_seq_array(new_cap);
        for (size_t i = 0; i < c_cap; ++i) new_c[i] = c_cache[i];
        if (c_cap == 0) {
            new_c[0] = 1;
            c_cap = 1;
        }
        for (idx_t k = (idx_t)c_cap; k <= n; ++k) {
            /* C(2k,k) = C(2(k-1),(k-1)) * (4k-2) / k */
            new_c[k] = (new_c[k - 1] * ((seq_t)(4 * k - 2))) / (seq_t)k;
        }
        if (c_cache != NULL) free_seq_array(c_cache);
        c_cache = new_c;
        c_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_size(n);
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        sum += c_cache[k] * p_cache[n - k];
    }
    return sum;
}
