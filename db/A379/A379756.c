/*
 * A379756  a(n) is the number of subsets of S(n) that sum to A023196(n),
 *          where S(n) is the set of the proper divisors (or aliquot parts) of
 *          A023196(n).
 * https://oeis.org/A379756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. k = A023196(n) is found by sieving sigma(k) >= 2k. With
 *            e = sigma(k) - 2k, a(n) = number of subsets of the divisors of k
 *            (including k) that sum to e. That equals A065205(k) (the subsets
 *            of proper divisors summing to e, which by complement is the same
 *            count as those summing to k), plus, wh...
 * Range:     u8 n <= 29, u16 n <= 176, u32 n <= 623, u64 n <= 3754,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9919 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t *nd_list = NULL;
static size_t nd_cnt = 0;

static void nd_build(size_t need)
{
    uint64_t lim = 1024;
    while (true) {
        uint64_t *sg = (uint64_t *)calloc((size_t)lim + 1, sizeof(uint64_t));
        if (!sg) abort();
        for (uint64_t i = 1; i <= lim; i++)
            for (uint64_t j = i; j <= lim; j += i) sg[j] += i;
        size_t c = 0;
        for (uint64_t k = 1; k <= lim; k++)
            if (sg[k] >= 2 * k) c++;
        if (c >= need) {
            free(nd_list);
            nd_list = (uint64_t *)malloc((c + 1) * sizeof(uint64_t));
            if (!nd_list) abort();
            nd_cnt = 0;
            for (uint64_t k = 1; k <= lim; k++)
                if (sg[k] >= 2 * k) nd_list[nd_cnt++] = k;
            free(sg);
            return;
        }
        free(sg);
        lim *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if ((size_t)n > nd_cnt) nd_build((size_t)n);
    uint64_t m = nd_list[n - 1];

    uint64_t divs[4096];
    size_t dc = 0;
    uint64_t sg = 0;
    for (uint64_t i = 1; i * i <= m; i++) {
        if (m % i == 0) {
            uint64_t j = m / i;
            sg += i;
            if (j != i) sg += j;
            if (i != m) divs[dc++] = i;
            if (j != i && j != m) divs[dc++] = j;
        }
    }
    uint64_t e = sg - 2 * m;
    uint64_t t1 = e < m ? e : m;          /* proper-divisor subsets summing to e (== to m) */
    uint64_t t2 = 0;                      /* proper-divisor subsets summing to e - m */
    bool use2 = (e >= m);
    if (use2) t2 = e - m;
    uint64_t L = t1 > t2 ? t1 : t2;

    seq_t *dp = mem_new(seq_t, (size_t)L + 1);
    dp[0] = 1;
    for (size_t t = 0; t < dc; t++) {
        uint64_t d = divs[t];
        if (d > L) continue;
        for (uint64_t s = L; s >= d; --s)
            dp[s] = dp[s] + dp[s - d];
    }
    seq_t r = dp[t1];
    if (use2) r = r + dp[t2];
    mem_free(dp);
    return r;
}
