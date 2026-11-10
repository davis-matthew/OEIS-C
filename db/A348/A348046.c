/*
 * A348046  Positions of 2's in A348045.
 * https://oeis.org/A348046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A348045(k) = 2, where A348045(k) = Sum_{d|k}
 *            mu(k/d)*A252463(d); computed by a sieve with a doubling limit
 * Range:     u8 n <= 15, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *pos_list = NULL;
static size_t pos_cnt = 0;

static void build_positions(size_t L)
{
    free(pos_list);
    pos_list = NULL;
    pos_cnt = 0;

    uint32_t *spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *prv = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    int8_t *mu = (int8_t *)calloc(L + 1, sizeof(int8_t));
    int64_t *A = (int64_t *)calloc(L + 1, sizeof(int64_t));
    int64_t *b = (int64_t *)calloc(L + 1, sizeof(int64_t));

    uint32_t last = 0;
    for (size_t i = 2; i <= L; ++i) {
        if (spf[i] == 0) {
            for (size_t j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            prv[i] = last;
            last = (uint32_t)i;
        }
    }
    mu[1] = 1;
    A[1] = 1;
    for (size_t i = 2; i <= L; ++i) {
        size_t p = spf[i], m = i / p;
        mu[i] = (m % p == 0) ? 0 : (int8_t)(-mu[m]);
        if ((i & 1) == 0) A[i] = (int64_t)(i / 2);
        else A[i] = A[m] * (int64_t)prv[p];
    }
    for (size_t d = 1; d <= L; ++d) {
        int64_t ad = A[d];
        for (size_t m = 1; d * m <= L; ++m) {
            if (mu[m]) b[d * m] += mu[m] * ad;
        }
    }
    size_t cap = 64;
    pos_list = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (size_t k = 1; k <= L; ++k) {
        if (b[k] == 2) {
            if (pos_cnt == cap) {
                cap *= 2;
                pos_list = (uint64_t *)realloc(pos_list, cap * sizeof(uint64_t));
            }
            pos_list[pos_cnt++] = k;
        }
    }
    free(spf); free(prv); free(mu); free(A); free(b);
}

static size_t cur_limit = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (pos_cnt < (size_t)n) {
        size_t L = cur_limit ? cur_limit * 4 : 4096;
        if (L < (size_t)n * 64) L = (size_t)n * 64;
        cur_limit = L;
        build_positions(L);
    }
    return (seq_t)pos_list[n - 1];
}
