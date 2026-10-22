/*
 * A300473  Numbers k with the property that k^2 + 21k + 1 is prime.
 * https://oeis.org/A300473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 168, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th positive integer k such that k^2+21k+1 is prime */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t sz = 0, cap = 0;
    static idx_t next_k = 1; /* next k to test */

    if (n <= sz) /* already computed */
        return cache[n - 1];

    while (sz < n)
    {
        seq_t k = (seq_t)next_k;
        seq_t val = k * k + (seq_t)21 * k + (seq_t)1;

        if (isPrimeGeneric(val))
        {
            if (sz == cap)
                grow_seq(&cache, &cap, cap ? cap * 2 : 4);
            cache[sz++] = k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
