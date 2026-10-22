/*
 * A344446  Number of partitions of n into 3 semiprimes.
 * https://oeis.org/A344446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of partitions of n into 3 semiprimes
 * Range:     u8 n <= 284, u16 n <= 5748, u32 n <= 9482, u64 n <= 9482,
 *            u128 n <= 9482, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static int cmp_idx_t(const void *p1, const void *p2)
{
    idx_t a = *(const idx_t *)p1;
    idx_t b = *(const idx_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

seq_t a(idx_t n)
{
    /* The smallest possible sum of three semiprimes is 4+4+4=12. */
    if (n < 12)
        return 0;

    /* 1. Generate primes up to n. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* 2. Build a boolean table of semiprimes up to n. */
    unsigned char *isSemi = (unsigned char *)calloc((size_t)(n + 1), sizeof(unsigned char));
    if (!isSemi) {
        free(primes);
        return 0;          /* allocation failure – return 0 as safe fallback */
    }

    /* Temporary array for collecting semiprime values. */
    size_t est = prime_cnt * (prime_cnt + 1) / 2 + 1;
    idx_t *tmp = (idx_t *)malloc(est * sizeof(idx_t));
    size_t tmp_cnt = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        for (size_t j = i; j < prime_cnt; ++j) {
            idx_t prod = (idx_t)primes[i] * (idx_t)primes[j];
            if (prod > n)
                break;
            if (!isSemi[prod])
                isSemi[prod] = 1;
            tmp[tmp_cnt++] = prod;
        }
    }

    free(primes);

    /* 3. Sort the unique semiprimes. */
    qsort(tmp, tmp_cnt, sizeof(idx_t), cmp_idx_t);
    /* Make a dedicated array for the sorted list. */
    idx_t *semis = (idx_t *)malloc(tmp_cnt * sizeof(idx_t));
    for (size_t i = 0; i < tmp_cnt; ++i)
        semis[i] = tmp[i];
    free(tmp);

    /* 4. Count unordered partitions i ≤ j ≤ k (semiprimes). */
    seq_t result = 0;
    for (size_t i = 0; i < tmp_cnt; ++i) {
        idx_t si = semis[i];
        for (size_t j = i; j < tmp_cnt; ++j) {
            idx_t sj = semis[j];
            idx_t sum2 = si + sj;
            if (sum2 > n)
                break;
            idx_t rem = n - sum2;
            if (rem < sj)
                continue;          /* enforce j ≤ k */
            if (rem <= n && isSemi[rem])
                ++result;         /* (si, sj, rem) is a valid partition */
        }
    }

    free(semis);
    free(isSemi);
    return result;
}
