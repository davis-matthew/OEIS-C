/*
 * A391992  Number of digits in the decimal expansion of binomial(2*n,n).
 * https://oeis.org/A391992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of decimal digits of binomial(2*n,n)
 * Range:     u8 n <= 426, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 89 terms through n = 4859 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* binomial(2n, n) is kept between calls in base-10^9 limbs that grow as
   needed, and stepped with C(2m+2, m+1) = C(2m, m) * 2(2m+1) / (m+1) (the
   division is exact); a smaller n restarts from C(0, 0) = 1. (The original
   recomputed it for every n and lost the array when realloc moved it.) */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *L = NULL;
static size_t len = 0, cap = 0;
static uint64_t m = 0;                       /* L holds binomial(2m, m) */

static void mul_small(uint64_t k)            /* k < 2^32 */
{
    uint64_t c = 0;
    for (size_t i = 0; i < len; ++i) {
        uint64_t p = (uint64_t)L[i] * k + c;
        L[i] = (uint32_t)(p % 1000000000u);
        c = p / 1000000000u;
    }
    while (c) {
        if (len == cap) {
            uint32_t *t = (uint32_t *)realloc(L, (cap *= 2) * sizeof *L);
            if (!t) abort();
            L = t;
        }
        L[len++] = (uint32_t)(c % 1000000000u);
        c /= 1000000000u;
    }
}

static void div_small(uint64_t k)
{
    uint64_t r = 0;
    for (size_t i = len; i-- > 0;) {
        uint64_t cur = L[i] + r * 1000000000u;
        L[i] = (uint32_t)(cur / k);
        r = cur % k;
    }
    while (len > 1 && L[len - 1] == 0) --len;
}

seq_t a(idx_t n)
{
    if (!L || m > n) {
        if (!L) {
            cap = 64;
            L = (uint32_t *)malloc(cap * sizeof *L);
            if (!L) abort();
        }
        L[0] = 1;
        len = 1;
        m = 0;
    }
    for (; m < n; ++m) {
        mul_small(2 * (2 * m + 1));
        div_small(m + 1);
    }
    uint32_t top = L[len - 1];
    int d = 0;
    while (top) { top /= 10; ++d; }
    return (seq_t)((len - 1) * 9 + (d ? d : 1));
}
