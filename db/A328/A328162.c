/*
 * A328162  Maximum length of a divisibility chain of consecutive divisors of
 *          n.
 * https://oeis.org/A328162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum length of a chain of consecutive divisors of n *
 *            where each divisor in the chain divides the next. Computed by
 *            enumerating * all divisors of n, sorting them, and scanning for
 *            the longest consecutive * divisible block
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t v1 = *(const uint64_t *)a;
    uint64_t v2 = *(const uint64_t *)b;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                               /* out of range */

    uint64_t N = (uint64_t)n;

    /* collect all divisors */
    size_t cap = 8;
    size_t len = 0;
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!divs)
        return 0;                               /* allocation failed */

    for (uint64_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            if (len == cap) {
                cap <<= 1;
                divs = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
                if (!divs)
                    return 0;
            }
            divs[len++] = i;
            uint64_t j = N / i;
            if (j != i) {
                if (len == cap) {
                    cap <<= 1;
                    divs = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
                    if (!divs)
                        return 0;
                }
                divs[len++] = j;
            }
        }
    }

    /* sort the divisors */
    qsort(divs, len, sizeof(uint64_t), cmp_uint64);

    /* scan for the longest consecutive divisible chain */
    size_t maxc = 1, curc = 1;
    for (size_t i = 1; i < len; ++i) {
        if (divs[i] % divs[i - 1] == 0)
            ++curc;
        else
            curc = 1;
        if (curc > maxc)
            maxc = curc;
    }

    free(divs);
    return (seq_t)maxc;
}
