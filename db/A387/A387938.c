/*
 * A387938  Fixed points of A390052.
 * https://oeis.org/A387938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th fixed point of A390052, i.e. A390052(a(n)) = a(n). *
 *            Computed by a search over each digit length d: a segmented sieve
 *            gives the * largest prime factor of every x < 10^d. Each x,
 *            written with d digits * (leading zeros allowed), is grouped by
 *            its digit multiset. The winner of a * group...
 * Range:     u8 n <= 79, u16 n <= 1537, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9993 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A387938_terms = NULL;
static size_t A387938_cnt = 0, A387938_cap = 0;
static int A387938_done_d = 0;

static int A387938_cmp_u64(const void *x, const void *y)
{
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return (p > q) - (p < q);
}

static bool A387938_push(uint64_t v)
{
    if (A387938_cnt == A387938_cap) {
        size_t nc = A387938_cap ? A387938_cap * 2 : 4096;
        uint64_t *p = (uint64_t *)realloc(A387938_terms, nc * sizeof(uint64_t));
        if (!p) return false;
        A387938_terms = p;
        A387938_cap = nc;
    }
    A387938_terms[A387938_cnt++] = v;
    return true;
}

static bool A387938_extend(int d)
{
    uint64_t N = 1, lowfull = 1;
    for (int i = 0; i < d; ++i) N *= 10;
    lowfull = (d == 1) ? 1 : N / 10;

    /* small primes up to sqrt(N) */
    uint64_t lim = 1;
    while (lim * lim < N) ++lim;
    char *comp = (char *)calloc((size_t)lim + 1, 1);
    uint64_t *pr = (uint64_t *)malloc(((size_t)lim + 1) * sizeof(uint64_t));
    if (!comp || !pr) { free(comp); free(pr); return false; }
    size_t np = 0;
    for (uint64_t i = 2; i <= lim; ++i) {
        if (!comp[i]) {
            pr[np++] = i;
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
        }
    }
    free(comp);

    /* hash table sized from C(d+9,9) */
    uint64_t ms = 1;
    for (int i = 1; i <= 9; ++i) ms = ms * (uint64_t)(d + i) / (uint64_t)i;
    size_t hsz = 1; int hbits = 0;
    while (hsz < 2 * ms + 16) { hsz <<= 1; ++hbits; }
    uint64_t *hk = (uint64_t *)calloc(hsz, sizeof(uint64_t));
    uint64_t *hl = (uint64_t *)calloc(hsz, sizeof(uint64_t));
    uint64_t *hv = (uint64_t *)calloc(hsz, sizeof(uint64_t));

    const size_t B = (size_t)1 << 15;
    uint64_t *rem = (uint64_t *)malloc(B * sizeof(uint64_t));
    uint64_t *big = (uint64_t *)malloc(B * sizeof(uint64_t));
    if (!hk || !hl || !hv || !rem || !big) {
        free(pr); free(hk); free(hl); free(hv); free(rem); free(big);
        return false;
    }

    for (uint64_t lo = 0; lo < N; lo += B) {
        uint64_t hi = lo + B; if (hi > N) hi = N;
        size_t len = (size_t)(hi - lo);
        for (size_t i = 0; i < len; ++i) { rem[i] = lo + i; big[i] = 1; }
        for (size_t k = 0; k < np; ++k) {
            uint64_t p = pr[k];
            if (p * p >= hi && p > 1) { if (p * p > hi) break; }
            uint64_t st = ((lo + p - 1) / p) * p;
            if (st == 0) st = p;
            for (uint64_t m = st; m < hi; m += p) {
                size_t idx = (size_t)(m - lo);
                while (rem[idx] % p == 0) rem[idx] /= p;
                big[idx] = p;
            }
        }
        for (size_t i = 0; i < len; ++i) {
            uint64_t x = lo + i;
            uint64_t lpf;
            if (x <= 1) lpf = 1;
            else lpf = (rem[i] > 1) ? rem[i] : big[i];
            uint64_t key = 0, t = x;
            for (int j = 0; j < d; ++j) { key += (uint64_t)1 << (4 * (t % 10)); t /= 10; }
            size_t h = (size_t)((key * 0x9E3779B97F4A7C15ULL) >> (64 - hbits));
            while (hk[h] != 0 && hk[h] != key) h = (h + 1) & (hsz - 1);
            if (hk[h] == 0) { hk[h] = key; hl[h] = lpf; hv[h] = x; }
            else if (lpf > hl[h]) { hl[h] = lpf; hv[h] = x; }
        }
    }

    size_t start = A387938_cnt;
    bool ok = true;
    for (size_t h = 0; h < hsz && ok; ++h)
        if (hk[h] != 0 && hv[h] >= lowfull && hv[h] < N)
            ok = A387938_push(hv[h]);
    if (ok)
        qsort(A387938_terms + start, A387938_cnt - start, sizeof(uint64_t), A387938_cmp_u64);

    free(pr); free(hk); free(hl); free(hv); free(rem); free(big);
    return ok;
}

seq_t a(idx_t n)
{
    while (A387938_cnt < (size_t)n) {
        if (A387938_done_d >= 19) return (seq_t)0;
        if (!A387938_extend(A387938_done_d + 1)) return (seq_t)0;
        ++A387938_done_d;
    }
    return (seq_t)A387938_terms[(size_t)n - 1];
}
