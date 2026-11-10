/*
 * A290078  Where the ratio A235027(n)/n obtains record values.
 * https://oeis.org/A290078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that A235027(k)/k > A235027(j)/j for all j<k;
 *            computed by a resumable segmented sieve of the completely
 *            multiplicative A235027 (product of bit-reversed prime factors,
 *            2->2)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define REC_CAP 1024
#define SEG 65536

static uint64_t rec_list[REC_CAP];
static size_t rec_cnt = 0;
static uint64_t next_lo = 1;
static uint64_t best_A = 0, best_n = 1;

static uint64_t *pr_list = 0;
static size_t pr_cnt = 0;
static uint64_t pr_lim = 0;

static uint64_t A235027_rev(uint64_t x)
{
    if (x == 2) return 2;
    uint64_t r = 0;
    while (x) { r = (r << 1) | (x & 1); x >>= 1; }
    return r;
}

static void ensure_primes(uint64_t need)
{
    if (need <= pr_lim) return;
    uint64_t lim = pr_lim ? pr_lim : 1024;
    while (lim < need) lim *= 2;
    char *comp = (char *)calloc((size_t)lim + 1, 1);
    free(pr_list);
    pr_list = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)lim / 2 + 2));
    pr_cnt = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!comp[i]) {
            pr_list[pr_cnt++] = i;
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
        }
    }
    free(comp);
    pr_lim = lim;
}

static void process_segment(void)
{
    static uint64_t rem[SEG], prod[SEG];
    uint64_t lo = next_lo, hi = lo + SEG;
    uint64_t sq = 1;
    while (sq * sq < hi) sq++;
    ensure_primes(sq + 1);
    for (uint64_t i = 0; i < SEG; i++) { rem[i] = lo + i; prod[i] = 1; }
    for (size_t k = 0; k < pr_cnt && pr_list[k] <= sq; k++) {
        uint64_t p = pr_list[k], rp = A235027_rev(p);
        uint64_t m = ((lo + p - 1) / p) * p;
        for (; m < hi; m += p) {
            uint64_t i = m - lo;
            while (rem[i] % p == 0) { rem[i] /= p; prod[i] *= rp; }
        }
    }
    for (uint64_t i = 0; i < SEG; i++) {
        uint64_t n = lo + i;
        uint64_t A = prod[i];
        if (rem[i] > 1) A *= A235027_rev(rem[i]);
        bool better;
        if (best_A == 0) better = true;
        else better = (unsigned __int128)A * best_n > (unsigned __int128)best_A * n;
        if (better) {
            best_A = A; best_n = n;
            if (rec_cnt < REC_CAP) rec_list[rec_cnt++] = n;
        }
    }
    next_lo = hi;
}

seq_t a(idx_t n)
{
    if (n == 0 || n > REC_CAP) return (seq_t)0;
    while (rec_cnt < (size_t)n) process_segment();
    return (seq_t)rec_list[n - 1];
}
