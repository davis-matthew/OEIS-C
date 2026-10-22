/*
 * A333262  Number of steps to reach 1 by iterating the alternating sum of
 *          divisors function A071324 starting from n.
 * https://oeis.org/A333262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n = 1; a(n) = a(A071324(n)) + 1 where A071324(n) is
 *            the absolute value of the alternating sum of the divisors of n
 *            sorted in ascending order, starting with a positive sign
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>

/* Comparison function for qsort of idx_t values. */
static int cmp_idx(const void *a, const void *b)
{
    idx_t x = *(const idx_t*)a;
    idx_t y = *(const idx_t*)b;
    return (x < y) - (x > y);
}

/* Return the alternating sum of the divisors of n, absolute value. */
static seq_t alt_sum_divisors(idx_t n)
{
    /* Small upper bound on the number of divisors for the expected range. */
    const size_t MAX_DIV = 300;
    idx_t divs[MAX_DIV];
    size_t cnt = 0;

    idx_t sq = (idx_t)sqrtl((long double)n);
    for (idx_t i = 1; i <= sq; ++i) {
        if (n % i == 0) {
            divs[cnt++] = i;
            idx_t j = n / i;
            if (j != i) divs[cnt++] = j;
        }
    }

    qsort(divs, cnt, sizeof(idx_t), cmp_idx);

    __int128 sum = 0;
    int sign = 1;
    for (size_t k = 0; k < cnt; ++k) {
        sum += (__int128)divs[k] * sign;
        sign = -sign;
    }

    return sum >= 0 ? (seq_t)sum : (seq_t)(-sum);
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    seq_t steps = 0;
    idx_t cur = n;

    while (cur != 1) {
        seq_t next = alt_sum_divisors(cur);
        cur = (idx_t)next;
        ++steps;
    }

    return steps;
}
