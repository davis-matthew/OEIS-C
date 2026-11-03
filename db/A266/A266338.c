/*
 * A266338  G.f. = b(2)*b(4)*b(6)/(x^8-x^3-x+1), where b(k) = (1-x^k)/(1-x).
 * https://oeis.org/A266338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies a(0)=1 and, for n≥1, a(n) = a(n-1) + a(n-3) –
 *            a(n-8) + c(n), where the inhomogeneity c(n) is c(0)=1, c(1)=3,
 *            c(2)=5, c(3)=7, c(4)=8, c(5)=8, c(6)=7, c(7)=5, c(8)=3, c(9)=1,
 *            and c(n)=0 for n≥10. This recurrence comes from the generating
 *            function G(x) = (1+x)(1+x+…+x³)(1+x+…+x⁵)/(1‑x...
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 57, u64 n <= 121, u128 n <= 250,
 *            big any n
 * Validated: standard, pass, 56 terms through n = 950 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* value of the inhomogeneous term c(n) for 0 ≤ n ≤ 9 */
static inline seq_t coeff_c(idx_t n)
{
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)3;
        case 2: return (seq_t)5;
        case 3: return (seq_t)7;
        case 4: return (seq_t)8;
        case 5: return (seq_t)8;
        case 6: return (seq_t)7;
        case 7: return (seq_t)5;
        case 8: return (seq_t)3;
        case 9: return (seq_t)1;
        default: return (seq_t)0;
    }
}

static seq_t *cache = NULL;   /* cache of computed terms */
static idx_t cache_len = 0;   /* size of allocated cache */

/* Main sequence function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Allocate or grow cache to store terms up to n */
    if (n >= cache_len) {
        size_t new_len = (cache_len > 0) ? cache_len * 2 : 32;
        if (new_len <= (size_t)n) new_len = (size_t)n + 1;

        seq_t *new_cache = mem_new(seq_t, new_len);
        if (!new_cache) return (seq_t)0;   /* allocation failed – unlikely */

        if (cache_len > 0) {
            mem_copy(new_cache, cache, cache_len * sizeof(seq_t));
            mem_free(cache);
        }
        cache = new_cache;
        cache_len = new_len;

        /* Initialise a(0) if we are starting from scratch */
        if (cache_len > 0 && cache[0] == (seq_t)0)
            cache[0] = (seq_t)1;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (i < cache_len && cache[i] != (seq_t)0) continue; /* already computed */

        seq_t term = (seq_t)0;

        /* a(n-1) component */
        term += cache[i - 1];

        /* a(n-3) component */
        if (i >= 3) term += cache[i - 3];

        /* subtract a(n-8) */
        if (i >= 8) term -= cache[i - 8];

        /* add the inhomogeneous part */
        term += coeff_c(i);

        cache[i] = term;
    }

    return cache[n];
}
