/*
 * A330757  Let d(1) < d(2) < ... < d(q) denote the divisors of n; a(n) is the
 *          number of elements of the set { d(1)/d(2), d(2)/d(3), ...,
 *          d(q-1)/d(q) }.
 * https://oeis.org/A330757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of distinct ratios of consecutive sorted
 *            divisors of n: a(n) = | { d[i]/d[i+1] : 1 ≤ i ≤ τ(n)-1 } |,
 *            where d[1] < ... < d[τ(n)] (OEIS A330757)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* local gcd for unsigned 64-bit integers */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0ULL) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* comparator for qsort on uint64_t values */
static int cmp_uint64(const void *p1, const void *p2)
{
    uint64_t x = *(const uint64_t *)p1;
    uint64_t y = *(const uint64_t *)p2;
    return (x > y) - (x < y);
}

seq_t a(idx_t n)
{
    /* cast to native integer for arithmetic (n fits in 64 bits) */
    uint64_t un = (uint64_t)n;

    /* collect divisors – capacity 256 is sufficient for n ≤ 10 000 */
    uint64_t divs[256];
    size_t cnt = 0;
    uint64_t i;
    for (i = 1ULL; i * i <= un; ++i) {
        if (un % i == 0ULL) {
            divs[cnt++] = i;
            if (i != un / i)
                divs[cnt++] = un / i;
        }
    }
    if (cnt == 0)   /* n = 0 would give zero, but n ≥ 1 by definition */
        return (seq_t)0;

    /* sort the divisors */
    qsort(divs, cnt, sizeof(uint64_t), cmp_uint64);

    /* enum distinct reduced ratios between consecutive divisors */
    struct { uint64_t num, den; } ratios[256];
    size_t uniq = 0;
    for (i = 0; i + 1 < cnt; ++i) {
        uint64_t num = divs[i];
        uint64_t den = divs[i + 1];
        uint64_t g = gcd_u64(num, den);
        num /= g;
        den /= g;

        /* check if this fraction is already seen */
        bool seen = false;
        for (size_t j = 0; j < uniq; ++j) {
            if (ratios[j].num == num && ratios[j].den == den) {
                seen = true;
                break;
            }
        }
        if (!seen) {
            ratios[uniq].num = num;
            ratios[uniq].den = den;
            ++uniq;
        }
    }

    return (seq_t)uniq;
}
