/*
 * A386989  Irregular triangle read by rows: T(n,k) is the product of terms in
 *          the k-th 2-dense sublist of divisors of n, with n >= 1, k >= 1.
 * https://oeis.org/A386989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened triangle of products of the maximal
 *            2‑dense sublists of the divisors of the row index. Each row m
 *            contributes one term for each such sublist, the value being the
 *            product of the divisors in that sublist
 * Range:     u8 n <= 18, u16 n <= 41, u32 n <= 120, u64 n <= 417,
 *            u128 n <= 1913, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/seq_mul.h"

/* local comparator for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* list of primes up to 100, sufficient for factorising all n <= 10000 */
static const uint64_t _primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};
static const size_t _prime_cnt = sizeof(_primes) / sizeof(_primes[0]);

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* no valid term for offset 0 */

    idx_t count = 0;                      /* number of generated terms */
    uint64_t m = 1;                        /* current row index */

    /* buffers reused for each m */
    uint64_t divs[2048];
    uint64_t f_primes[64];
    int f_exps[64];

    for (;; ++m)
    {
        /* factorise m */
        uint64_t rem = m;
        size_t fp_cnt = 0;

        for (size_t pi = 0; pi < _prime_cnt; ++pi)
        {
            uint64_t p = _primes[pi];
            if (p * p > rem) break;
            if (rem % p == 0)
            {
                int exp = 0;
                while (rem % p == 0) { rem /= p; ++exp; }
                f_primes[fp_cnt]  = p;
                f_exps[fp_cnt++]  = exp;
            }
        }
        if (rem > 1)                /* remaining prime factor */
        {
            f_primes[fp_cnt]  = rem;
            f_exps[fp_cnt++]  = 1;
        }

        /* generate all divisors */
        size_t div_cnt = 1;          /* start with divisor 1 */
        divs[0] = 1;

        for (size_t fi = 0; fi < fp_cnt; ++fi)
        {
            uint64_t p = f_primes[fi];
            int    e = f_exps[fi];

            size_t sz = div_cnt;
            uint64_t pow = 1;
            for (int k = 1; k <= e; ++k)
            {
                pow *= p;
                for (size_t j = 0; j < sz; ++j)
                {
                    divs[div_cnt++] = divs[j] * pow;
                }
            }
        }

        /* sort the divisor list */
        qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

        /* split into maximal 2‑dense sublists and emit products */
        size_t i = 0;
        while (i < div_cnt)
        {
            seq_t prod = (seq_t)1;
            prod = seq_mul(prod, (seq_t)divs[i]);
            size_t j = i + 1;
            while (j < div_cnt && divs[j] <= divs[j - 1] * 2ULL)
            {
                prod = seq_mul(prod, (seq_t)divs[j]);
                ++j;
            }
            ++count;
            if (count == n) return prod;
            i = j;
        }
    }
}
