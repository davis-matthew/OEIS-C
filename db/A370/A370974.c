/*
 * A370974  A260850 sorted into increasing order and duplicates omitted.
 * https://oeis.org/A370974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve-like generation: compute A260850(1..M) via the exponent
 *            recurrence a(k) = a(k-1) * prod p^(+-e) (multiply p^e if e >
 *            v_p(a(k-1)), else divide), keep every term that fits in seq_t,
 *            sort ascending and remove duplicates; a(n) is the n-th smallest
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 69, u128 n <= 129,
 *            big any n
 * Validated: standard, pass, 56 terms through n = 3221 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *A370974_sorted = NULL;
static size_t A370974_count = 0;
static size_t A370974_built = 0;

static void A370974_msort(seq_t *v, size_t m)
{
    seq_t *tmp = mem_new(seq_t, m ? m : 1);
    for (size_t w = 1; w < m; w <<= 1) {
        for (size_t lo = 0; lo < m; lo += 2 * w) {
            size_t mid = lo + w; if (mid > m) mid = m;
            size_t hi = lo + 2 * w; if (hi > m) hi = m;
            size_t i = lo, j = mid, k = lo;
            while (i < mid && j < hi) {
                if (v[j] < v[i]) { tmp[k++] = v[j++]; }
                else { tmp[k++] = v[i++]; }
            }
            while (i < mid) tmp[k++] = v[i++];
            while (j < hi) tmp[k++] = v[j++];
        }
        for (size_t i = 0; i < m; ++i) v[i] = tmp[i];
    }
    mem_free(tmp);
}

/* multiply *val by p^e with overflow detection; returns false on overflow */
static bool A370974_mulpow(seq_t *val, uint32_t p, int e)
{
    for (int r = 0; r < e; ++r) {
        seq_t nv = *val * (seq_t)p;
        if (nv / (seq_t)p != *val) return false;
        *val = nv;
    }
    return true;
}

static void A370974_build(size_t M)
{
    uint32_t *spf = (uint32_t *)malloc(sizeof(uint32_t) * (M + 1));
    int *ex = (int *)calloc(M + 1, sizeof(int));
    uint32_t *plist = (uint32_t *)malloc(sizeof(uint32_t) * (M + 1));
    if (!spf || !ex || !plist) abort();
    size_t np = 0;
    for (size_t i = 0; i <= M; ++i) spf[i] = 0;
    for (size_t i = 2; i <= M; ++i) {
        if (spf[i] == 0) {
            plist[np++] = (uint32_t)i;
            for (size_t j = i; j <= M; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    seq_t *vals = mem_new(seq_t, M);
    size_t cnt = 0;
    seq_t val = 1;
    bool valid = true;
    vals[cnt++] = val;
    for (size_t i = 2; i <= M; ++i) {
        uint32_t mp[24]; int me[24]; int mc = 0;
        uint32_t dp[24]; int de[24]; int dc = 0;
        size_t t = i;
        while (t > 1) {
            uint32_t p = spf[t];
            int e = 0;
            while (t % p == 0) { t /= p; ++e; }
            if (e > ex[p]) { ex[p] += e; mp[mc] = p; me[mc] = e; ++mc; }
            else { ex[p] -= e; dp[dc] = p; de[dc] = e; ++dc; }
        }
        if (valid) {
            for (int k = 0; k < dc; ++k)
                for (int r = 0; r < de[k]; ++r) val = val / (seq_t)dp[k];
            for (int k = 0; k < mc && valid; ++k)
                if (!A370974_mulpow(&val, mp[k], me[k])) valid = false;
        }
        if (!valid) {
            /* recompute from the exponent vector, stopping on overflow */
            val = 1;
            valid = true;
            for (size_t k = 0; k < np && valid; ++k) {
                uint32_t p = plist[k];
                if (ex[p] > 0 && !A370974_mulpow(&val, p, ex[p])) valid = false;
            }
        }
        if (valid) vals[cnt++] = val;
    }
    free(spf);
    free(ex);
    free(plist);
    A370974_msort(vals, cnt);
    size_t u = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (u == 0 || vals[u - 1] < vals[i]) { vals[u++] = vals[i]; }
    }
    if (A370974_sorted) mem_free(A370974_sorted);
    A370974_sorted = vals;
    A370974_count = u;
    A370974_built = M;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t need = 3 * (size_t)n + 200;
    if (A370974_built < need) A370974_build(need);
    if ((size_t)n > A370974_count) return (seq_t)0;
    return A370974_sorted[n - 1];
}
