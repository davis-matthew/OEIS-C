/*
 * A353355  Numbers k for which A353328(k) is equal to A353329(k).
 * https://oeis.org/A353355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that #{d|k : A332823(d)=1} = #{d|k :
 *            A332823(d)=-1}, i.e. Sum_{d|k} A332823(d) = 0 (found by sieve +
 *            divisor-class counting)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t *A353355_terms = NULL;
static size_t A353355_cnt = 0;
static size_t A353355_limit = 0;

static void A353355_build(size_t L)
{
    uint32_t *spf = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
    uint8_t *wt = (uint8_t *)malloc((L + 1) * sizeof(uint8_t));
    size_t i, j;
    for (i = 0; i <= L; ++i) { spf[i] = 0; wt[i] = 0; }
    for (i = 2; i <= L; ++i) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    {
        size_t idx = 0;
        for (i = 2; i <= L; ++i)
            if (spf[i] == i) {
                wt[i] = (idx & 1) ? 2 : 1; /* weight mod 3: +1 or -1 */
                ++idx;
            }
    }
    free(A353355_terms);
    A353355_terms = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
    A353355_cnt = 0;
    for (i = 1; i <= L; ++i) {
        long long c[3] = {1, 0, 0};
        size_t m = i;
        while (m > 1) {
            uint32_t p = spf[m];
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            {
                long long nc[3] = {0, 0, 0};
                int t, r;
                for (t = 0; t <= e; ++t) {
                    int add = (int)((t * (int)wt[p]) % 3);
                    for (r = 0; r < 3; ++r)
                        nc[(r + add) % 3] += c[r];
                }
                c[0] = nc[0]; c[1] = nc[1]; c[2] = nc[2];
            }
        }
        if (c[1] == c[2])
            A353355_terms[A353355_cnt++] = (uint64_t)i;
    }
    A353355_limit = L;
    free(spf);
    free(wt);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A353355_limit == 0) A353355_build(1024);
    while (A353355_cnt < (size_t)n)
        A353355_build(A353355_limit * 2);
    return (seq_t)A353355_terms[(size_t)n - 1];
}
