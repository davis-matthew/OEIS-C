/*
 * A339686  a(n) = Sum_{d|n} 6^(d-1).
 * https://oeis.org/A339686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 6^(d-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 979 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *pow_cache = NULL;  /* 6^k for k = 0 .. N-1 */
static size_t pow_cap   = 0;     /* current cache capacity */

/* Computes a(n) = Σ_{d|n} 6^(d-1)  for n ≥ 1.
   The result fits into seq_t on the provided build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard against 0 */

    /* grow power cache if necessary */
    if (pow_cap < (size_t)n)
    {
        size_t new_cap = n;
        seq_t *tmp = mem_resize(seq_t, pow_cache, new_cap);
        if (!tmp) return 0;          /* bad allocation – return 0 in this context */
        pow_cache = tmp;

        if (pow_cap == 0)
        {
            /* initialise the first element and fill the rest */
            pow_cache[0] = 1;
            for (size_t i = 1; i < new_cap; ++i)
                pow_cache[i] = mul_small(pow_cache[i - 1], 6);
        }
        else
        {
            /* extend the cache */
            for (size_t i = pow_cap; i < new_cap; ++i)
                pow_cache[i] = mul_small(pow_cache[i - 1], 6);
        }
        pow_cap = new_cap;
    }

    /* sum contributions of all divisors of n */
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d)
    {
        if (n % d == 0)
            sum += pow_cache[(size_t)(d - 1)];
    }
    return sum;
}
