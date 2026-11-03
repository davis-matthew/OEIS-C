/*
 * A396263  G.f. A(x) satisfies: A(x) = 1 + x * A(2*x)^2 / (1 - x).
 * https://oeis.org/A396263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 77 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A396263: a(0)=1,
   for n>0: a(n)=∑_{j=0}^{n-1}∑_{i=0}^{j} 2^j·a(i)·a(j-i) */
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    /* static cached arrays for repeated calls */
    static seq_t *a_cache = NULL;
    static size_t a_cap = 0;
    static seq_t *c_cache = NULL;
    static size_t c_cap = 0;

    /* make sure the arrays are large enough */
    ensure_cache(n, &a_cache, &a_cap);
    ensure_cache(n, &c_cache, &c_cap);

    /* initial values */
    a_cache[0] = 1;
    c_cache[0] = 1;          /* a0 * a0 */

    seq_t pref = 1;          /* pref = 2^0 * c0 = 1 */
    seq_t powTwo = 1;        /* current 2^k, starts with 2^0 */

    for (idx_t k = 1; k <= n; ++k) {
        /* a(k) = current pref */
        a_cache[k] = pref;

        /* compute c(k) = sum_{i=0}^{k} a(i)*a(k-i) */
        seq_t sum = 0;
        idx_t half = k / 2;
        if ((k & 1) == 0) {       /* even k */
            seq_t midProd = a_cache[half] * a_cache[half];
            sum = midProd;
            for (idx_t i = 0; i < half; ++i) {
                seq_t prod = a_cache[i] * a_cache[k - i];
                sum += prod;
                sum += prod;      /* double */
            }
        } else {                  /* odd k */
            for (idx_t i = 0; i <= half; ++i) {
                seq_t prod = a_cache[i] * a_cache[k - i];
                sum += prod;
                sum += prod;
            }
        }
        c_cache[k] = sum;

        /* update pref for next iteration: pref += 2^k * c(k) */
        powTwo = powTwo * 2;                               /* 2^{k} from 2^{k-1} */
        pref += powTwo * sum;
    }

    return a_cache[n];
}
