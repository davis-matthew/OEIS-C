/*
 * A271556  a(n) = G_n(9), where G is the Goodstein function defined in
 *          A266201.
 * https://oeis.org/A271556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = G_n(9) where G is the Goodstein function. * Recurrence:
 *            a(0) = 9; a(n) = B_{n+1}(a(n-1)) – 1 * B_{b}(x) expands x in
 *            hereditary base‑b, replaces each * occurrence of base b by b+1
 *            in all places (including exponents), * evaluates the resulting
 *            expression, and returns the value
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   alloc_seq, pow_seq_large, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../utils/alloc_seq.h"
#include "../utils/seq_free.h"
#include "../utils/pow_seq_large.h"

/* Recursively compute B_b(x).  base and x are arbitrary‑precision integers. */
static seq_t B_helper(seq_t base, seq_t x)
{
    if (x < base)
        return x;

    seq_t result = 0;
    idx_t i = 0;
    seq_t t = x;

    while (t > 0) {
        seq_t digit = t % base;
        t = t / base;
        if (digit != 0) {
            seq_t exp_out = B_helper(base, (seq_t)i);
            seq_t pow_val = pow_seq_large(base + 1, exp_out);
            result += digit * pow_val;
        }
        ++i;
    }
    return result;
}

/* Compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n < cache_cap)
        return cache[n];

    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = alloc_seq(new_cap);

    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        seq_free(cache);
    }

    for (idx_t k = cache_cap; k <= n; ++k) {
        if (k == 0) {
            new_cache[0] = (seq_t)9;
        } else {
            seq_t val = B_helper((seq_t)(k + 1), new_cache[k - 1]);
            new_cache[k] = val - 1;
        }
    }

    cache = new_cache;
    cache_cap = new_cap;
    return cache[n];
}
