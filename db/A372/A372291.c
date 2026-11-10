/*
 * A372291  Numbers that occur exactly once in the odd bisection of A371094.
 * https://oeis.org/A372291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number (increasing) occurring exactly once in the
 *            array A(n,k)=4^n*(6k-3-2*(-1)^n)+(4^n-1)/3, n,k>=1 (A371100);
 *            computed by sieving value multiplicities up to a doubling limit
 * Range:     u8 n <= 7, u16 n <= 2181, u32 n <= 25365, u64 n <= 25365,
 *            u128 n <= 25365, big any n
 * Validated: standard, pass, 75 terms through n = 8616 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A372291_terms = NULL;
static uint64_t A372291_cnt = 0;

static void A372291_build(uint64_t L)
{
    unsigned char *c = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t n, k, p, v;
    free(A372291_terms);
    A372291_terms = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)L + 1));
    A372291_cnt = 0;
    for (n = 1, p = 4; ; n++, p *= 4) {
        uint64_t add = (p - 1) / 3;
        uint64_t base = (n & 1) ? 1 : 5;   /* 6k - base' : odd n -> 6k-1, even n -> 6k-5 */
        uint64_t first = p * ((n & 1) ? 5 : 1) + add;
        if (first > L) break;
        (void)base;
        for (k = 1; ; k++) {
            uint64_t coef = (n & 1) ? 6 * k - 1 : 6 * k - 5;
            v = p * coef + add;
            if (v > L) break;
            if (c[v] < 2) c[v]++;
        }
    }
    for (v = 1; v <= L; v++)
        if (c[v] == 1) A372291_terms[A372291_cnt++] = v;
    free(c);
}

seq_t a(idx_t n)
{
    static uint64_t L = 0;
    if (L == 0) { L = 4096; A372291_build(L); }
    while (A372291_cnt < (uint64_t)n) {
        L *= 2;
        A372291_build(L);
    }
    return (seq_t)A372291_terms[n - 1];
}
