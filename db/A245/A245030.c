/*
 * A245030  Divisors of 7^24 - 1.
 * https://oeis.org/A245030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest divisor of 7^24-1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 1574, u64 n <= 9207,
 *            u128 n <= 9207, big any n
 * Helpers:   cmp_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmp_divisors.h"

/* Helper function to generate all divisors recursively. */
static void generate_divisors(int idx, seq_t cur,
        const seq_t primes[], const unsigned int exps[],
        seq_t *divs, size_t *pos, int num_primes)
{
    if (idx == num_primes) {
        divs[(*pos)++] = cur;
        return;
    }
    seq_t val = cur;
    for (unsigned int e = 0; e <= exps[idx]; ++e) {
        generate_divisors(idx + 1, val, primes, exps, divs, pos, num_primes);
        val = val * primes[idx];
    }
}

seq_t a(idx_t n)
{
    /* Static storage for all divisors of 7^24-1. */
    static seq_t *divs = NULL;
    static size_t div_count = 0;

    if (!divs) {
        /* Prime factorisation of 7^24-1. */
        static const seq_t primes[] =
            { (seq_t)2, (seq_t)3, (seq_t)5,
              (seq_t)13, (seq_t)19, (seq_t)43,
              (seq_t)73, (seq_t)181, (seq_t)193,
              (seq_t)409, (seq_t)1201 };
        static const unsigned int exps[] =
            { 6, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1 };
        const int num_primes = sizeof(primes) / sizeof(primes[0]);

        /* Compute total number of divisors. */
        div_count = 1;
        for (int i = 0; i < num_primes; ++i)
            div_count *= (exps[i] + 1);

        divs = (seq_t *)malloc(div_count * sizeof(seq_t));
        if (!divs) { /* Allocation failed. Return 0 as safe fallback. */
            return (seq_t)0;
        }

        /* Generate all divisors. */
        size_t pos = 0;
        generate_divisors(0, (seq_t)1, primes, exps, divs, &pos, num_primes);

        /* Sort the divisors in ascending order. */
        qsort(divs, div_count, sizeof(seq_t), cmp_divisors);
    }

    if (n < 1 || n > div_count)
        return (seq_t)0; /* Undefined out of range. */
    return divs[n - 1];
}
