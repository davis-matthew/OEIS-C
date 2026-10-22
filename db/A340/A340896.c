/*
 * A340896  Irregular triangle in which the n-th row consists of all multiples
 *          of n that have fewer than twice as many divisors as n.
 * https://oeis.org/A340896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A340896: the n-th term of the irregular triangle where row r consists of
   all multiples of r that have fewer than twice as many divisors as r.
   Rows are listed sequentially; the indexing follows the OEIS definition. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* comparator used by qsort for seq_t values */
static int seq_compare(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    idx_t cum = 0;                     /* number of entries seen so far */

    for (idx_t r = 1; ; ++r)           /* iterate over rows */
    {
        /* factorise r */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t pcnt;
        factor_u64((uint64_t)r, primes, exps, &pcnt);

        /* number of elements in this row: 1 (the n itself) + sum of exponents */
        size_t row_cnt = 1;
        for (size_t i = 0; i < pcnt; ++i)
            row_cnt += (size_t)exps[i];

        /* allocate array to hold the row values */
        seq_t *row = (seq_t *)malloc(row_cnt * sizeof(seq_t));
        if (!row) return 0;            /* out of memory guard */

        size_t pos = 0;
        row[pos++] = (seq_t)r;         /* the number r itself */

        /* each prime factor may be increased in exponent up to its original
           exponent (doubling the exponent keeps the divisor count below 2*d(r)) */
        for (size_t i = 0; i < pcnt; ++i) {
            seq_t mult = (seq_t)r;          /* start from r */
            seq_t p    = (seq_t)primes[i];
            for (uint64_t delta = 1; delta <= exps[i]; ++delta) {
                mult *= p;
                row[pos++] = mult;
            }
        }

        /* sort the row – the number of elements is small */
        qsort(row, row_cnt, sizeof(seq_t), seq_compare);

        /* check if the desired index falls into this row */
        if (cum + row_cnt >= n) {
            seq_t result = row[n - cum - 1];
            free(row);
            return result;
        }

        cum += row_cnt;
        free(row);
    }
}
