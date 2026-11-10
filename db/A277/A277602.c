/*
 * A277602  List of indices n such that A092488(n) < A068527(n).
 * https://oeis.org/A277602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m with A092488(m) < A068527(m); computed by blocks
 *            between consecutive squares, using GF(2) dependency of the
 *            numbers k^2+1..(k+1)^2-1, scanned downward from the top of each
 *            block
 * Range:     u8 n <= 60, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 70 terms through n = 4642 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define A277602_MAXK 200000

static uint64_t A277602_cum[A277602_MAXK + 1];
static uint64_t A277602_K = 0;

/* squarefree-part prime list of m (primes with odd exponent), ascending */
static size_t A277602_oddprimes(uint64_t m, uint64_t *out)
{
    size_t cnt = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e ^= 1; }
            if (e) out[cnt++] = p;
        }
    }
    if (m > 1) out[cnt++] = m;
    return cnt;
}

static size_t A277602_symdiff(const uint64_t *a, size_t la, const uint64_t *b, size_t lb, uint64_t *out)
{
    size_t i = 0, j = 0, k = 0;
    while (i < la && j < lb) {
        if (a[i] < b[j]) out[k++] = a[i++];
        else if (a[i] > b[j]) out[k++] = b[j++];
        else { ++i; ++j; }
    }
    while (i < la) out[k++] = a[i++];
    while (j < lb) out[k++] = b[j++];
    return k;
}

static uint64_t A277602_block(uint64_t k)
{
    uint64_t lo = k * k + 1, hi = (k + 1) * (k + 1) - 1;
    if (lo > hi) return 0;
    size_t cap = (size_t)(hi - lo + 1);
    uint64_t **bd = (uint64_t **)malloc(cap * sizeof(uint64_t *));
    size_t *bl = (size_t *)malloc(cap * sizeof(size_t));
    size_t bc = 0;
    uint64_t result = 0;
    for (uint64_t m = hi; m >= lo; --m) {
        uint64_t init[64];
        size_t len = A277602_oddprimes(m, init);
        uint64_t *v = (uint64_t *)malloc((len + 1) * sizeof(uint64_t));
        for (size_t i = 0; i < len; ++i) v[i] = init[i];
        bool dep = false;
        for (;;) {
            if (len == 0) { dep = true; break; }
            size_t bi = 0;
            while (bi < bc && bd[bi][0] != v[0]) ++bi;
            if (bi == bc) break;
            uint64_t *t = (uint64_t *)malloc((len + bl[bi] + 1) * sizeof(uint64_t));
            size_t tl = A277602_symdiff(v, len, bd[bi], bl[bi], t);
            free(v);
            v = t;
            len = tl;
        }
        if (dep) {
            free(v);
            result = m - k * k;
            break;
        }
        bd[bc] = v;
        bl[bc] = len;
        ++bc;
    }
    for (size_t i = 0; i < bc; ++i) free(bd[i]);
    free(bd);
    free(bl);
    return result;
}

seq_t a(idx_t n)
{
    uint64_t t = (uint64_t)n;
    if (A277602_K == 0) { A277602_cum[0] = 0; }
    uint64_t k = 1;
    for (;;) {
        while (A277602_K < k) {
            uint64_t kk = A277602_K + 1;
            A277602_cum[kk] = A277602_cum[kk - 1] + A277602_block(kk);
            A277602_K = kk;
        }
        if (A277602_cum[k] >= t) break;
        ++k;
        if (k >= A277602_MAXK) return (seq_t)0;
    }
    return (seq_t)(k * k + (t - A277602_cum[k - 1]));
}
