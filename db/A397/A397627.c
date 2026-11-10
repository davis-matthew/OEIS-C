/*
 * A397627  a(n) = prime(A026351(n)).
 * https://oeis.org/A397627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(A026351(n-1)), A026351(m) = floor(m*phi) + 1, phi =
 *            (1+sqrt(5))/2
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t A026351_isqrt(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq = mid * mid;
        if (sq == v) return mid;
        if (sq < v) lo = mid + 1;
        else {
            if (mid == 0) return 0;
            hi = mid - 1;
        }
    }
    return hi;
}

/* A026351, offset 0: a(m) = floor(m*phi) + 1 */
static seq_t A026351_a(idx_t m)
{
    seq_t ms = (seq_t)m;
    seq_t t = ms * ms * (seq_t)5;
    seq_t s = A026351_isqrt(t);
    return (ms + s) / 2 + (seq_t)1;
}

static uint64_t *A397627_pr = NULL;
static size_t A397627_cnt = 0;
static uint64_t A397627_lim = 0;

static void A397627_ensure(size_t k)
{
    while (A397627_cnt < k) {
        uint64_t lim = A397627_lim < 64 ? 64 : A397627_lim * 2;
        unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
        uint64_t i, j;
        size_t cnt = 0;
        uint64_t *pr;
        if (!comp) return;
        for (i = 2; i * i <= lim; i++)
            if (!comp[i])
                for (j = i * i; j <= lim; j += i)
                    comp[j] = 1;
        for (i = 2; i <= lim; i++)
            if (!comp[i]) cnt++;
        pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        if (!pr) { free(comp); return; }
        cnt = 0;
        for (i = 2; i <= lim; i++)
            if (!comp[i]) pr[cnt++] = i;
        free(comp);
        free(A397627_pr);
        A397627_pr = pr;
        A397627_cnt = cnt;
        A397627_lim = lim;
    }
}

seq_t a(idx_t n)
{
    seq_t idx;
    size_t k;
    if (n < 1) return (seq_t)0;
    idx = A026351_a(n - 1);
    k = (size_t)(long long)idx;
    A397627_ensure(k);
    if (A397627_cnt < k) return (seq_t)0;
    return (seq_t)A397627_pr[k - 1];
}
