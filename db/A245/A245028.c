/*
 * A245028  Divisors of 11^12 - 1.
 * https://oeis.org/A245028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th divisor of 11^12 – 1, sorted in ascending order
 *            (OEIS A245028)
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 1254, u64 n <= 1920,
 *            u128 n <= 1920, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Comparator used by qsort */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Recursive generation of all divisors */
static void gen_divs(size_t depth, seq_t current,
                     const uint64_t *pr, const int *ex,
                     size_t pcnt, seq_t *arr, size_t *idx)
{
    if (depth == pcnt) {
        arr[*idx] = current;
        (*idx)++;
        return;
    }
    seq_t mult = 1;
    for (int e = 0; e <= ex[depth]; ++e) {
        gen_divs(depth + 1, current * mult, pr, ex, pcnt, arr, idx);
        mult *= (seq_t)pr[depth];
    }
}

seq_t a(idx_t n)
{
    static seq_t *divs = NULL;
    static size_t total = 0;
    static int initialized = 0;

    if (!initialized) {
        /* Prime factorisation of 11^12 – 1:
           2^4 * 3^2 * 5 * 7 * 13 * 19 * 37 * 61 * 1117 */
        const uint64_t primes[]   = {2ULL,3ULL,5ULL,7ULL,13ULL,19ULL,37ULL,61ULL,1117ULL};
        const int     exponents[] = {4,2,1,1,1,1,1,1,1};
        const size_t  pcnt = sizeof(primes)/sizeof(primes[0]);

        total = 1;
        for (size_t i = 0; i < pcnt; ++i)
            total *= (size_t)(exponents[i] + 1);   /* 1920 divisors */

        divs = (seq_t*)malloc(total * sizeof(seq_t));

        size_t idx = 0;
        gen_divs(0, (seq_t)1, primes, exponents, pcnt, divs, &idx);

        qsort(divs, total, sizeof(seq_t), seq_cmp);

        initialized = 1;
    }

    if (n < 1 || (size_t)(n - 1) >= total)
        return 0;   /* undefined for n outside valid range */

    return divs[(size_t)(n - 1)];
}
