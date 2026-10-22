/*
 * A360679  Sum of the right half (inclusive) of the prime indices of n.
 * https://oeis.org/A360679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the right half (inclusive) of the prime indices of
 *            n
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   cmp_seq_t, factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"
#include "../utils/cmp_seq_t.h"

/* Compute A360679: sum of the right half (inclusive) of the prime indices of n. */
seq_t a(idx_t n)
{
    if (n <= 1)                 /* a(1) = 0 */
        return 0;

    /* Factorise n into distinct primes and their exponents. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    /* Store each prime index (counted with multiplicity). */
    seq_t indices[128];
    size_t len = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t idx = prime_pi(pr[i]);   /* prime index of pr[i] */
        size_t e   = ex[i];
        for (size_t j = 0; j < e; ++j)
            indices[len++] = idx;
    }

    if (len == 0)
        return 0;

    /* Sort the indices to identify the right half. */
    qsort(indices, len, sizeof(seq_t), cmp_seq_t);

    /* Sum the right half (inclusive). */
    size_t start = len / 2;     /* ceil(len/2) from the right */
    seq_t sum = 0;
    for (size_t i = start; i < len; ++i)
        sum += indices[i];

    return sum;
}
