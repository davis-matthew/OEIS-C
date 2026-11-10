/*
 * A367357  First differences of A134647.
 * https://oeis.org/A367357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A134647(n+1) - A134647(n), where A134647(1)=0,
 *            A134647(2)=1, A134647(n) = A134647(n-1) + 10*(A134647(n-2) mod
 *            10) + firstdigit(A134647(n-1))
 * Range:     u8 n <= 8, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A134647_a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 2;
    }

    while (cache_size < (size_t)n) {
        if (cache_size + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (tmp == NULL) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }
        seq_t prev2 = cache[cache_size - 2];
        seq_t prev1 = cache[cache_size - 1];
        seq_t last = prev2 % ((seq_t)10);
        seq_t first = prev1;
        while (first >= ((seq_t)10)) first /= (seq_t)10;
        seq_t form = last * ((seq_t)10) + first;
        cache[cache_size] = prev1 + form;
        cache_size++;
    }
    return cache[n - 1];
}

seq_t a(idx_t n)
{
    return A134647_a(n + 1) - A134647_a(n);
}
