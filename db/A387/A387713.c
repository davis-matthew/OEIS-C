/*
 * A387713  Nonprimitive terms of A387711: numbers k for which A387715(k) > 1.
 * https://oeis.org/A387713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with #{d | k : A003959(d) > 2*d} > 1, where
 *            A003959 is completely multiplicative on primes with
 *            A003959(p)=p+1 (sieve with doubling limit)
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9190 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *np_list = NULL;
static size_t np_len = 0;
static uint64_t np_limit = 0;

static void np_build(uint64_t limit)
{
    uint32_t *spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    uint64_t *A = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    unsigned char *cnt = (unsigned char *)calloc(limit + 1, 1);
    uint32_t *lst = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf || !A || !cnt || !lst) abort();
    for (uint64_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    A[0] = 0;
    if (limit >= 1) A[1] = 1;
    for (uint64_t i = 2; i <= limit; ++i)
        A[i] = A[i / spf[i]] * ((uint64_t)spf[i] + 1);
    for (uint64_t d = 1; d <= limit; ++d) {
        if (A[d] > 2 * d) {
            for (uint64_t m = d; m <= limit; m += d)
                if (cnt[m] < 2) cnt[m]++;
        }
    }
    size_t len = 0;
    for (uint64_t k = 1; k <= limit; ++k)
        if (cnt[k] > 1) lst[len++] = (uint32_t)k;
    free(spf);
    free(A);
    free(cnt);
    free(np_list);
    np_list = lst;
    np_len = len;
    np_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (np_limit == 0) np_build(4096);
    while ((uint64_t)np_len < (uint64_t)n) np_build(np_limit * 2);
    return (seq_t)np_list[n - 1];
}
