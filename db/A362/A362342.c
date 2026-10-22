/*
 * A362342  a(n) = n! * Sum_{k=0..floor(n/4)} (-k/24)^k / (k! * (n-4*k)!).
 * https://oeis.org/A362342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/4⌋} (-k/24)^k / (k! * (n-4k)!)
 * Range:     i8 n <= 8, i16 n <= 11, i32 n <= 16, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Euclidean gcd for unsigned 64‑bit integers */
static inline uint64_t ugcd(uint64_t a, uint64_t b)
{
    while (b)
    {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = n / 4;
    /* pre‑allocate maximum needed space for numerator and denominator arrays */
    size_t max_num = 5 * (size_t)kmax;
    size_t max_den = 2 * (size_t)kmax;
    uint64_t *num = (uint64_t *)malloc(sizeof(uint64_t) * max_num);
    uint64_t *den = (uint64_t *)malloc(sizeof(uint64_t) * max_den);
    if (!num || !den) /* minimal error handling */
        return 0;

    for (idx_t k = 0; k <= kmax; ++k)
    {
        if (k == 0)
        {
            sum += 1;
            continue;
        }

        size_t num_cnt = 5 * (size_t)k;
        size_t den_cnt = 2 * (size_t)k;

        /* fill numerator array: falling factorial n*(n-1)*...*(n-4k+1) and k copies of k */
        for (size_t i = 0; i < 4 * (size_t)k; ++i)
            num[i] = (uint64_t)(n - (idx_t)i);
        for (size_t i = 4 * (size_t)k; i < num_cnt; ++i)
            num[i] = (uint64_t)k;

        /* fill denominator array: k! and 24^k (24 repeated k times) */
        for (size_t i = 0; i < (size_t)k; ++i)
            den[i] = (uint64_t)(i + 1);
        for (size_t i = 0; i < (size_t)k; ++i)
            den[(size_t)k + i] = 24;

        /* cancel common factors between numerator and denominator */
        for (size_t di = 0; di < den_cnt; ++di)
        {
            uint64_t d = den[di];
            for (size_t ni = 0; ni < num_cnt && d > 1; ++ni)
            {
                uint64_t g = ugcd(num[ni], d);
                if (g > 1)
                {
                    num[ni] /= g;
                    d /= g;
                }
            }
            /* after this loop d should be 1 (integer division validates that the term is an integer) */
        }

        /* multiply remaining numerator factors (they now form the reduced term) */
        __int128 prod = 1;
        for (size_t i = 0; i < num_cnt; ++i)
            prod *= (__int128)num[i];

        seq_t term;
        if ((k & 1) == 0)
            term = (seq_t)prod;
        else
            term = -(seq_t)prod;

        sum += term;
    }

    free(num);
    free(den);
    return sum;
}
