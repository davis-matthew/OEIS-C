/*
 * A342436  a(n) = Sum_{k=1..n} gcd(k,n)^(gcd(k,n) - 1).
 * https://oeis.org/A342436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * d^(d-1) (OEIS A342436)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 386 (2026-11-03)
 * Helpers:   ipow_idx, mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/ipow_idx.h"
#include "../utils/totient.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* n = 0 is outside the sequence range */
    if (n == 0)
        return 0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = mem_new(seq_t, new_cap);
        if (!tmp)
            return 0;
        if (cache) {
            mem_copy(tmp, cache, cache_cap * sizeof(seq_t));
            mem_free(cache);
        }
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    if (cache[n - 1] != 0)
        return cache[n - 1];

    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if ((n % d) == 0) {
            seq_t phi_val = totient(n / d);     /* phi from utils */
            seq_t pow_val = ipow_idx((seq_t)d, d - 1);
            sum += phi_val * pow_val;
        }
    }

    cache[n - 1] = sum;
    return sum;
}
