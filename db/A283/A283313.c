/*
 * A283313  Smallest missing number after A283312(n) has been found.
 * https://oeis.org/A283313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   simulation of A283312: a(1)=1; A283312(n) = 2*A283312(n-1) if
 *            A283312(n-1) is prime, otherwise the smallest number not yet
 *            used. a(n) = smallest positive integer missing from
 *            {A283312(1..n)}
 * Range:     u8 n <= 277, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 95 terms through n = 9680 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *res_cache = NULL;
static idx_t res_cap = 0;

static void build(idx_t N)
{
    size_t L = (size_t)(2 * N + 16);
    bool *used = (bool *)calloc(L, sizeof(bool));
    bool *comp = (bool *)calloc(L, sizeof(bool));
    uint64_t *res = (uint64_t *)malloc((size_t)(N + 1) * sizeof(uint64_t));
    size_t i, j;
    comp[0] = comp[1] = true;
    for (i = 2; i * i < L; i++)
        if (!comp[i])
            for (j = i * i; j < L; j += i) comp[j] = true;

    uint64_t prev = 0, m = 1;
    idx_t k;
    for (k = 1; k <= N; k++) {
        uint64_t v;
        if (prev > 1 && !comp[prev]) v = 2 * prev;
        else v = m;
        used[v] = true;
        while (used[m]) m++;
        res[k] = m;
        prev = v;
    }
    free(used);
    free(comp);
    free(res_cache);
    res_cache = res;
    res_cap = N;
}

seq_t a(idx_t n)
{
    if (n > res_cap) {
        idx_t N = res_cap ? res_cap : 64;
        while (N < n) N *= 2;
        build(N);
    }
    return (seq_t)res_cache[n];
}
