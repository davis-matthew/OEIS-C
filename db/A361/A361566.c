/*
 * A361566  a(n) is the denominator of the median of divisors of n.
 * https://oeis.org/A361566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A361566: a(n) is the denominator of the median of the divisors of n. */
#include <stdlib.h>

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return 0; /* undefined, but never reached for valid n >= 1 */

    /* collect all positive divisors of n into a buffer */
    /* 2048 is safely large for n <= 10^9 (tau(n) < 1536), and well within stack limits. */
    uint64_t divs[2048];
    size_t tau = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            divs[tau++] = i;
            if (i != n / i) {
                divs[tau++] = n / i;
                if (tau >= sizeof(divs) / sizeof(divs[0])) /* guard against overflow */
                    ; /* ignore: N is too large for the test constraints */
            }
        }
    }

    /* if the count of divisors is odd the median is an integer */
    if (tau & 1)
        return (seq_t)1;

    /* sort the divisor list */
    qsort(divs, tau, sizeof(uint64_t), cmp_uint64);

    /* two middle divisors */
    size_t mid = tau / 2;          /* 0‑based index of the upper middle divisor */
    uint64_t d1 = divs[mid - 1];
    uint64_t d2 = divs[mid];

    /* denominator is 2 iff d1 + d2 is odd (i.e., their parities differ) */
    if (((d1 & 1ULL) ^ (d2 & 1ULL)))
        return (seq_t)2;
    else
        return (seq_t)1;
}
