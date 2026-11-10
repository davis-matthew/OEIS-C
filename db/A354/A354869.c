/*
 * A354869  Positions of odd terms in A354866, the Dirichlet inverse of
 *          A122111.
 * https://oeis.org/A354869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m such that A354866(m) is odd, where A354866 is the
 *            Dirichlet inverse of A122111. A122111(k) is odd iff k=1 or the
 *            largest prime factor of k divides k at least twice. So parity:
 *            b(1)=1, b(m) = sum_{d|m, d<m} g(m/d) b(d) mod 2, with g(k)=1 iff
 *            that condition holds. Computed by a sieve...
 * Range:     u8 n <= 32, u16 n <= 1234, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 83 terms through n = 7332 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t *pos_list = NULL;
static size_t pos_cnt = 0;

static void build_positions(uint64_t N)
{
    uint32_t *lp = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    if (!lp) abort();
    for (uint64_t p = 2; p <= N; ++p) {
        if (lp[p] == 0) {
            for (uint64_t m = p; m <= N; m += p) lp[m] = (uint32_t)p;
        }
    }
    size_t kc = 0;
    for (uint64_t k = 2; k <= N; ++k) {
        uint32_t p = lp[k];
        if (((k / p) % p) == 0) ++kc;
    }
    uint32_t *K = (uint32_t *)malloc((kc + 1) * sizeof(uint32_t));
    if (!K) abort();
    size_t j = 0;
    for (uint64_t k = 2; k <= N; ++k) {
        uint32_t p = lp[k];
        if (((k / p) % p) == 0) K[j++] = (uint32_t)k;
    }
    free(lp);

    uint8_t *b = (uint8_t *)calloc((size_t)N + 1, 1);
    if (!b) abort();
    b[1] = 1;
    size_t cap = 1024, cnt = 0;
    uint64_t *res = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!res) abort();
    for (uint64_t d = 1; d <= N; ++d) {
        if (!b[d]) continue;
        if (cnt == cap) {
            cap <<= 1;
            res = (uint64_t *)realloc(res, cap * sizeof(uint64_t));
            if (!res) abort();
        }
        res[cnt++] = d;
        for (size_t i = 0; i < kc; ++i) {
            uint64_t m = d * (uint64_t)K[i];
            if (m > N) break;
            b[m] ^= 1;
        }
    }
    free(b);
    free(K);
    free(pos_list);
    pos_list = res;
    pos_cnt = cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    static uint64_t lim = 0;
    while (pos_cnt < (size_t)n) {
        lim = lim ? lim * 2 : 4096;
        build_positions(lim);
    }
    return (seq_t)pos_list[(size_t)n - 1];
}
