/*
 * A338537  Numbers that occur more than once in A338529.
 * https://oeis.org/A338537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number (increasing) that occurs more than once in
 *            A338529, where A338529(m) = prime(m+2)*prime(m+3) -
 *            prime(m)*prime(m+1). Computed by sieve: all A338529 values <= V
 *            are generated (A338529(m) > 4*prime(m+2)), sorted, and values
 *            with multiplicity >= 2 collected; V is enlarged until eno...
 * Range:     u8 none, u16 n <= 2, u32 n <= 609, u64 n <= 609, u128 n <= 609,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 587 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *dup_vals = NULL;
static size_t dup_cnt = 0;
static uint64_t cur_V = 0;

static int cmp_u64_a338537(const void *x, const void *y)
{
    uint64_t a1 = *(const uint64_t *)x, b1 = *(const uint64_t *)y;
    return (a1 > b1) - (a1 < b1);
}

static void build_dups_a338537(uint64_t V)
{
    uint64_t L = V / 4 + 2000;
    unsigned char *comp = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)L / 2 + 16));
    size_t pc = 0;
    uint64_t i, j;
    for (i = 2; i <= L; i++) {
        if (!comp[i]) {
            pr[pc++] = i;
            for (j = i * i; j <= L; j += i) comp[j] = 1;
        }
    }
    free(comp);

    uint64_t *vals = (uint64_t *)malloc(sizeof(uint64_t) * (pc + 1));
    size_t vc = 0;
    size_t k;
    for (k = 0; k + 3 < pc; k++) {
        if (4 * pr[k + 2] > V) break;
        uint64_t v = pr[k + 2] * pr[k + 3] - pr[k] * pr[k + 1];
        if (v <= V) vals[vc++] = v;
    }
    free(pr);
    qsort(vals, vc, sizeof(uint64_t), cmp_u64_a338537);

    free(dup_vals);
    dup_vals = (uint64_t *)malloc(sizeof(uint64_t) * (vc + 1));
    dup_cnt = 0;
    k = 0;
    while (k < vc) {
        size_t e = k + 1;
        while (e < vc && vals[e] == vals[k]) e++;
        if (e - k >= 2) dup_vals[dup_cnt++] = vals[k];
        k = e;
    }
    free(vals);
    cur_V = V;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (cur_V == 0) build_dups_a338537((uint64_t)1 << 22);
    while ((uint64_t)n > dup_cnt) {
        build_dups_a338537(cur_V * 4);
    }
    return (seq_t)dup_vals[(size_t)(n - 1)];
}
