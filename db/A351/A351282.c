/*
 * A351282  a(n) = Sum_{k=0..n} 3^k * k^(n-k).
 * https://oeis.org/A351282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * k^(n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 552 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *term = NULL;          /* term[k] = 3^k * k^(current_n-k)   */
    static seq_t *a_cache = NULL;       /* a_cache[i] = a(i) */
    static size_t term_cap = 0;
    static size_t a_cap = 0;
    static idx_t cur_n = 0;
    static seq_t pow3 = 0;              /* 3^cur_n */

    /* One‑time initialisation */
    if (term == NULL) {
        term_cap = 1;
        term = mem_new(seq_t, term_cap);
        term[0] = 1;                    /* 3^0 * 0^0 = 1 */
        a_cap = 1;
        a_cache = mem_new(seq_t, a_cap);
        a_cache[0] = 1;                  /* a(0) = 1 */
        pow3 = 1;                       /* 3^0 */
    }

    /* Expand stored values up to requested n if necessary */
    if (n > cur_n) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > term_cap) {
            term_cap = new_cap;
            term = mem_resize(seq_t, term, term_cap);
        }
        if (new_cap > a_cap) {
            a_cap = new_cap;
            a_cache = mem_resize(seq_t, a_cache, a_cap);
        }

        for (idx_t i = cur_n + 1; i <= n; ++i) {
            /* Update existing terms: term_k *= k for k < i */
            for (idx_t k = 0; k < i; ++k) {
                term[k] = mul_small(term[k], (unsigned int)k);
            }
            /* Compute new power of three */
            pow3 = mul_small(pow3, 3);
            /* New term for k = i */
            term[i] = pow3;
            /* Sum to obtain a(i) */
            seq_t sum = 0;
            for (idx_t k = 0; k <= i; ++k) {
                sum += term[k];
            }
            a_cache[i] = sum;
        }
        cur_n = n;
    }

    return a_cache[n];
}
