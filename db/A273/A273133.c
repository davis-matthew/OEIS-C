/*
 * A273133  a(n) = n minus the bottom entry of the difference table of the
 *          divisors of n.
 * https://oeis.org/A273133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - A187202(n) (the bottom of the difference table of the
 *            divisors of n)
 * Range:     i8 n <= 47, i16 n <= 179, i32 n <= 1259, i64 n <= 7559,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"   /* provides: void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

/* Local comparator for qsort on seq_t values. */
static int seq_cmp(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Compute a(n) as specified. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, guard against underflow */

    /* 1. Factor n into prime powers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    /* 2. Compute the divisor count. */
    size_t divisor_cnt = 1;
    for (size_t i = 0; i < prime_cnt; ++i)
        divisor_cnt *= (size_t)(exps[i] + 1);

    /* 3. Allocate array for divisors. */
    seq_t *divs = (seq_t *)malloc(divisor_cnt * sizeof(seq_t));
    if (!divs) return (seq_t)0;    /* allocation failure – fallback */

    /* 4. Generate all divisors (sorted later). */
    divs[0] = (seq_t)1;
    size_t used = 1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        uint64_t e = exps[pi];

        size_t prev_used = used;
        seq_t power = (seq_t)1;
        for (uint64_t exp = 1; exp <= e; ++exp) {
            power = power * (seq_t)p;
            for (size_t j = 0; j < prev_used; ++j)
                divs[used++] = divs[j] * power;
        }
    }

    /* 5. Sort divisors ascending. */
    qsort(divs, divisor_cnt, sizeof(seq_t), seq_cmp);

    /* 6. Compute the bottom entry of the difference table. */
    size_t m = divisor_cnt;
    idx_t k = (idx_t)(m > 0 ? m - 1 : 0);   /* k = m-1 */

    seq_t coeff = (seq_t)1;  /* C(k,0) */
    seq_t bottom = (seq_t)0;

    for (size_t i = 0; i < m; ++i) {
        /* Sign = (-1)^(k-i) */
        int sign = (((k - (idx_t)i) & 1) == 0) ? 1 : -1;

        seq_t term = coeff * divs[i];
        if (sign > 0)
            bottom += term;
        else
            bottom -= term;

        /* Update binomial coefficient for next i, only if i < k. */
        if (i < (size_t)k) {
            seq_t num = (seq_t)(k - (idx_t)i);
            seq_t den = (seq_t)((idx_t)i + 1);
            coeff = coeff * num / den;
        }
    }

    free(divs);

    /* 7. Return a(n) = n - bottom. */
    return (seq_t)n - bottom;
}
