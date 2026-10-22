/*
 * A361320  If n is composite, replace n with the concatenation of its
 *          nontrivial divisors, written in increasing order, each divisor
 *          being written in base 10 with its digits in reverse order,
 *          otherwise a(n) = n.
 * https://oeis.org/A361320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   if n composite, a(n) is the concatenation of the * reversed
 *            decimal digits of its nontrivial divisors (written in *
 *            increasing order). Otherwise, a(n) = n
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Comparator for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t v1 = *(const uint64_t *)a;
    uint64_t v2 = *(const uint64_t *)b;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)n;               /* a(1) = 1 */

    /* collect nontrivial divisors of n */
    uint64_t divs[128];
    size_t cnt = 0;
    for (idx_t i = 2; i <= n / i; ++i)        /* avoid overflow */
    {
        if (n % i == 0)
        {
            divs[cnt++] = (uint64_t)i;
            if (i != n / i)
                divs[cnt++] = (uint64_t)(n / i);
        }
    }

    if (cnt == 0)                          /* n is prime */
        return (seq_t)n;

    /* sort divisors in ascending order */
    qsort(divs, cnt, sizeof(uint64_t), cmp_uint64);

    /* build the concatenated string of reversed divisors */
    char buf[1024];
    int pos = 0;
    for (size_t k = 0; k < cnt; ++k)
    {
        uint64_t v = divs[k];
        char tmp[32];
        int t = 0;
        do
        {
            tmp[t++] = '0' + (v % 10);     /* reversed digits of v */
            v /= 10;
        } while (v);
        for (int j = 0; j < t; ++j)
            buf[pos++] = tmp[j];
    }
    buf[pos] = '\0';

    /* convert buf to seq_t value */
    seq_t res = 0;
    for (int i = 0; i < pos; ++i)
        res = res * 10 + (buf[i] - '0');

    return res;
}
