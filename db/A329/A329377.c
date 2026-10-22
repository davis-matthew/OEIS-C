/*
 * A329377  Number of iterations done when n is divided by its divisors
 *          starting from the smallest one in increasing order until one no
 *          longer gets an integer, or until divisors are exhausted.
 * https://oeis.org/A329377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of successive divisions of n by its divisors in
 *            increasing order until the quotient is no longer an integer
 * Range:     u8 n <= 9983, u16 n <= 9983, u32 n <= 9983, u64 n <= 9983,
 *            u128 n <= 9983, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int cmp_uint64(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x;
    uint64_t b = *(const uint64_t *)y;
    return (a < b) ? -1 : (a > b);
}

seq_t a(idx_t n)
{
    /* Guard against dummy values; the sequence is defined for n >= 1. */
    if (n == 0) return 0;

    uint64_t N = (uint64_t)n;

    /* Estimate divisor count; the highly composite number < 10^12 has < 6000 divisors,
       which is far below the capacity of a small buffer. We grow the buffer if needed. */
    size_t cap = 128;
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!divs) return 0;        /* Out‑of‑memory: return 0 */

    size_t cnt = 0;
    /* Divisor 1 */
    divs[cnt++] = 1;

    /* Enumerate divisors by checking up to sqrt(N) */
    for (uint64_t d = 2; d * d <= N; ++d)
    {
        if (N % d == 0)
        {
            uint64_t q = N / d;
            /* Add d */
            if (cnt == cap) { cap <<= 1; divs = (uint64_t *)realloc(divs, cap * sizeof(uint64_t)); }
            divs[cnt++] = d;
            /* If d != q, add the co‑divisor */
            if (d != q)
            {
                if (cnt == cap) { cap <<= 1; divs = (uint64_t *)realloc(divs, cap * sizeof(uint64_t)); }
                divs[cnt++] = q;
            }
        }
    }

    /* Include N itself (if not already added) */
    if (N != 1)
    {
        if (cnt == cap) { cap <<= 1; divs = (uint64_t *)realloc(divs, cap * sizeof(uint64_t)); }
        divs[cnt++] = N;
    }

    /* Sort divisors */
    qsort(divs, cnt, sizeof(uint64_t), cmp_uint64);

    /* Count iterations */
    uint64_t cur = N;
    seq_t count = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t d = divs[i];
        if (cur % d == 0)
        {
            cur /= d;
            ++count;
        }
        else
            break;
    }

    free(divs);
    return count;
}
