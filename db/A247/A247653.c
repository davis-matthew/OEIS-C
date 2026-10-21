/*
 * A247653  Write 2n as a product of numbers of the form 4k+2 (A016825) so as
 *          to minimize the sum of the factors; a(n) is this sum.
 * https://oeis.org/A247653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_cmp_desc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Let 2n = 2^t * m with m odd.
   We must write m as a product of t odd numbers whose sum is minimal.
   Achieve this by distributing the prime factors of m in
   descending order to the currently smallest factor.
   The answer a(n) is then 2 * (minimal sum). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_cmp_desc.h"

seq_t a(idx_t n)
{
    /* compute t = number of 2's in 2n, and odd part m */
    idx_t tmp = n;
    idx_t t = 1;            /* at least one 2 in 2n */
    while ((tmp & 1ULL) == 0ULL) {
        tmp >>= 1;
        t++;
    }
    seq_t odd = (seq_t)tmp; /* m */

    /* factor the odd part */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)odd, primes, exps, &cnt);

    /* build list of prime factors (with multiplicity) */
    seq_t factors[64];
    size_t nf = 0;
    for (size_t i = 0; i < cnt; ++i) {
        for (uint64_t j = 0; j < exps[i]; ++j) {
            factors[nf++] = (seq_t)primes[i];
        }
    }

    if (nf > 1)
        qsort(factors, nf, sizeof(seq_t), seq_cmp_desc); /* descending */

    /* initialize t factors to 1 */
    seq_t parts[64] = {0};
    for (idx_t i = 0; i < t; ++i)
        parts[i] = (seq_t)1;

    /* distribute prime factors */
    for (size_t i = 0; i < nf; ++i) {
        seq_t f = factors[i];              /* current prime factor */
        /* find index of smallest current part */
        idx_t min_idx = 0;
        seq_t min_val = parts[0];
        for (idx_t j = 1; j < t; ++j) {
            if (parts[j] < min_val) {
                min_val = parts[j];
                min_idx = j;
            }
        }
        parts[min_idx] *= f;
    }

    /* sum the parts and double it */
    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i < t; ++i)
        sum += parts[i];

    return sum * (seq_t)2;
}
