/*
 * A262502  a(n) = least k such that A261104(k) = n; positions of records in
 *          A261104.
 * https://oeis.org/A262502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that A261104(k) = n, found by sieve: compute
 *            A261104(k) = 1 + A261104(k - A070319(k)) with A261104(0) = 0,
 *            and record the first k at which each value n appears. The limit
 *            doubles until n is reached
 * Range:     u8 n <= 22, u16 n <= 811, u32 n <= 1641, u64 n <= 1641,
 *            u128 n <= 1641, big any n
 * Validated: standard, pass, 89 terms through n = 1437 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t *rec_pos = NULL;
static idx_t rec_cnt = 0;
static idx_t cur_lim = 0;

static void rec_build(idx_t L)
{
    uint32_t *tau = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint32_t *val = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    idx_t *rp = (idx_t *)calloc((size_t)L + 2, sizeof(idx_t));
    if (!tau || !val || !rp) abort();
    for (idx_t i = 1; i <= L; ++i)
        for (idx_t j = i; j <= L; j += i)
            ++tau[j];
    idx_t cnt = 1;
    rp[0] = 0;
    uint32_t dmax = 0;
    for (idx_t k = 1; k <= L; ++k) {
        if (tau[k] > dmax) dmax = tau[k];
        val[k] = 1u + val[k - dmax];
        while ((idx_t)val[k] >= cnt) {
            rp[cnt] = k;
            ++cnt;
        }
    }
    free(rec_pos);
    rec_pos = rp;
    rec_cnt = cnt;
    cur_lim = L;
    free(tau);
    free(val);
}

seq_t a(idx_t n)
{
    while (n >= rec_cnt) {
        idx_t L = cur_lim ? cur_lim * 2 : 4096;
        rec_build(L);
    }
    return (seq_t)rec_pos[n];
}
