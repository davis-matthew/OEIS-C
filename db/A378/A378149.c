/*
 * A378149  a(n) is the number of distinct integer-sided right trapezoids with
 *          exactly one pair of parallel sides and area n.
 * https://oeis.org/A378149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts integer‑sided right trapezoids of area n by
 *            iterating over divisors h of 2*n and checking for Pythagorean
 *            triples with legs h and |p‑q|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 64‑bit unsigned integer */
static unsigned long long isqrt64(unsigned long long x)
{
    unsigned long long lo = 0, hi = 0xFFFFFFFFULL;
    while (lo <= hi) {
        unsigned long long mid = (lo + hi) >> 1;
        unsigned long long div = x / mid;
        if (mid <= div) {
            /* mid*mid <= x, check if (mid+1)^2 > x */
            if (mid == 0xFFFFFFFFULL) return mid;
            unsigned long long div_next = x / (mid + 1);
            if (mid + 1 > div_next) return mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* add trapezoids for a given height h */
static void add_height(idx_t h, idx_t two, seq_t *res)
{
    idx_t S = two / h;          /* sum of the two bases */
    if (S <= 1) return;         /* need at least one base > 0 */

    idx_t start = (S & 1) ? 1 : 2;   /* parity match with S */
    for (idx_t c = start; c < S; c += 2) {   /* candidate |p‑q| */
        unsigned long long hh = (unsigned long long)h;
        unsigned long long cc = (unsigned long long)c;
        unsigned long long t  = hh * hh + cc * cc;   /* h^2 + c^2 */
        unsigned long long d  = isqrt64(t);
        if (d * d == t) {          /* perfect square → valid d */
            (*res) += 1;           /* p = (S + c)/2, q = (S − c)/2  */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t two = n * 2;
    seq_t count = 0;

    for (idx_t i = 1; i <= two / i; ++i) {
        if (two % i != 0) continue;
        idx_t h1 = i;
        idx_t h2 = two / i;
        add_height(h1, two, &count);
        if (h2 != h1) add_height(h2, two, &count);
    }
    return count;
}
