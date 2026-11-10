/*
 * A357763  Numbers m such that A357761(m) > A357761(k) for all k < m.
 * https://oeis.org/A357763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search. a(n) is the n-th record position of A357761, where
 *            A357761(m) = sum_{d|m} (+1 if popcount(d) is odd, else -1).
 *            Since A357761(2^k*o) = (k+1)*A357761(o) for odd o, every record
 *            has the form 2^k*o with o an odd record. Odd records are found
 *            in the dyadic blocks (X/2, X] by a depth-first sear...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 48 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint64_t o; long long g; } A357763_og;

static uint64_t *A357763_P = 0;
static size_t A357763_NP = 0;
static uint64_t A357763_Plim = 0;

static void A357763_ext(uint64_t need)
{
    if (need <= A357763_Plim) return;
    uint64_t lim = A357763_Plim * 2;
    if (lim < need) lim = need;
    if (lim < 1024) lim = 1024;
    char *s = (char *)calloc((size_t)lim + 1, 1);
    for (uint64_t i = 2; i * i <= lim; i++)
        if (!s[i])
            for (uint64_t j = i * i; j <= lim; j += i) s[j] = 1;
    size_t cnt = 0;
    for (uint64_t i = 3; i <= lim; i += 2) if (!s[i]) cnt++;
    uint64_t *P = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (uint64_t i = 3; i <= lim; i += 2) if (!s[i]) P[cnt++] = i;
    free(s);
    free(A357763_P);
    A357763_P = P;
    A357763_NP = cnt;
    A357763_Plim = lim;
}

static uint64_t A357763_isqrt(uint64_t x)
{
    uint64_t r = 0;
    for (int b = 31; b >= 0; b--) {
        uint64_t c = r | ((uint64_t)1 << b);
        if (c * c <= x) r = c;
    }
    return r;
}

static inline long long A357763_chi(uint64_t v)
{
    return (__builtin_popcountll(v) & 1) ? 1 : -1;
}

static uint64_t A357763_X, A357763_half;
static long long A357763_T;

static uint64_t *A357763_div = 0;
static size_t A357763_divcap = 0;
static unsigned char A357763_seg[1 << 15];

static A357763_og *A357763_cand = 0;
static size_t A357763_cn = 0, A357763_cc = 0;

static void A357763_push(uint64_t o, long long g)
{
    if (A357763_cn == A357763_cc) {
        A357763_cc = A357763_cc ? A357763_cc * 2 : 1024;
        A357763_cand = (A357763_og *)realloc(A357763_cand, A357763_cc * sizeof(A357763_og));
    }
    A357763_cand[A357763_cn].o = o;
    A357763_cand[A357763_cn].g = g;
    A357763_cn++;
}

/* primes p in (lo, hi], leaves o = s*p */
static void A357763_leaf(uint64_t s, uint64_t lo, uint64_t hi, size_t off, size_t D, long long gs)
{
    const uint64_t S = (uint64_t)sizeof(A357763_seg);
    uint64_t b = lo + 1;
    if (b < 3) b = 3;
    while (b <= hi) {
        uint64_t e = (hi - b >= S - 1) ? b + S - 1 : hi;
        size_t len = (size_t)(e - b + 1);
        memset(A357763_seg, 0, len);
        for (size_t k = 0; k < A357763_NP; k++) {
            uint64_t q = A357763_P[k];
            if (q * q > e) break;
            uint64_t m = (b + q - 1) / q * q;
            if (m < q * q) m = q * q;
            if (!(m & 1)) m += q;
            for (; m <= e; m += 2 * q) A357763_seg[m - b] = 1;
        }
        for (uint64_t x = b | 1; x <= e; x += 2) {
            if (A357763_seg[x - b]) continue;
            long long g = gs;
            for (size_t i = 0; i < D; i++) g += A357763_chi(A357763_div[off + i] * x);
            if (g > A357763_T) A357763_push(s * x, g);
        }
        if (e == hi) break;
        b = e + 1;
    }
}

static void A357763_dfs(size_t j0, uint64_t o, uint64_t lastp, size_t off, size_t D, long long gs)
{
    if (o > A357763_half && gs > A357763_T) A357763_push(o, gs);
    uint64_t R = A357763_X / o;
    int pruned = 0;
    for (size_t j = j0; j < A357763_NP; j++) {
        uint64_t p = A357763_P[j];
        if (p > R / p) break;
        int w = 0;
        uint64_t q = R;
        while (q >= p) { q /= p; w++; }
        if (gs + (long long)((((uint64_t)1) << w) - 1) * (long long)D <= A357763_T) { pruned = 1; break; }
        size_t noff = off + D;
        uint64_t t = o;
        for (int e = 1; t <= A357763_X / p; e++) {
            t *= p;
            size_t nD = D * (size_t)(e + 1);
            if (noff + nD > A357763_divcap) {
                while (noff + nD > A357763_divcap) A357763_divcap *= 2;
                A357763_div = (uint64_t *)realloc(A357763_div, A357763_divcap * sizeof(uint64_t));
            }
            long long ng = 0;
            size_t k = 0;
            uint64_t pp = 1;
            for (int i = 0; i <= e; i++) {
                for (size_t u = 0; u < D; u++) {
                    uint64_t v = A357763_div[off + u] * pp;
                    A357763_div[noff + k++] = v;
                    ng += A357763_chi(v);
                }
                if (i < e) pp *= p;
            }
            A357763_dfs(j + 1, t, p, noff, nD, ng);
        }
    }
    if (!pruned && gs + (long long)D > A357763_T) {
        uint64_t lo = lastp;
        uint64_t sq = A357763_isqrt(R);
        if (sq > lo) lo = sq;
        uint64_t h = A357763_half / o;
        if (h > lo) lo = h;
        if (lo < R) A357763_leaf(o, lo, R, off, D, gs);
    }
}

static int A357763_cmp(const void *x, const void *y)
{
    uint64_t a = ((const A357763_og *)x)->o, b = ((const A357763_og *)y)->o;
    return a < b ? -1 : (a > b);
}

static A357763_og *A357763_orec = 0;
static size_t A357763_on = 0, A357763_oc = 0;
static long long A357763_G = 0;

static uint64_t *A357763_terms = 0;
static size_t A357763_nt = 0;
static uint64_t A357763_cur = 0;

static void A357763_addrec(uint64_t o, long long g)
{
    if (A357763_on == A357763_oc) {
        A357763_oc = A357763_oc ? A357763_oc * 2 : 256;
        A357763_orec = (A357763_og *)realloc(A357763_orec, A357763_oc * sizeof(A357763_og));
    }
    A357763_orec[A357763_on].o = o;
    A357763_orec[A357763_on].g = g;
    A357763_on++;
    A357763_G = g;
}

static void A357763_stage(uint64_t X)
{
    A357763_X = X;
    A357763_half = X / 2;
    A357763_T = A357763_G;
    A357763_cn = 0;
    A357763_ext(A357763_isqrt(X) + 2);
    if (!A357763_div) {
        A357763_divcap = (size_t)1 << 20;
        A357763_div = (uint64_t *)malloc(A357763_divcap * sizeof(uint64_t));
    }
    A357763_div[0] = 1;
    A357763_dfs(0, 1, 2, 0, 1, 1);
    if (A357763_cn) qsort(A357763_cand, A357763_cn, sizeof(A357763_og), A357763_cmp);
    for (size_t i = 0; i < A357763_cn; i++)
        if (A357763_cand[i].g > A357763_G)
            A357763_addrec(A357763_cand[i].o, A357763_cand[i].g);
}

static void A357763_rebuild(uint64_t L)
{
    size_t cap = 0, cnt = 0;
    A357763_og *arr = 0;
    for (size_t i = 0; i < A357763_on; i++) {
        uint64_t m = A357763_orec[i].o;
        for (int k = 0; m <= L; k++) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                arr = (A357763_og *)realloc(arr, cap * sizeof(A357763_og));
            }
            arr[cnt].o = m;
            arr[cnt].g = (long long)(k + 1) * A357763_orec[i].g;
            cnt++;
            if (m > L / 2) break;
            m *= 2;
        }
    }
    if (cnt) qsort(arr, cnt, sizeof(A357763_og), A357763_cmp);
    free(A357763_terms);
    A357763_terms = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    A357763_nt = 0;
    long long best = 0;
    for (size_t i = 0; i < cnt; i++) {
        if (arr[i].g > best) {
            best = arr[i].g;
            A357763_terms[A357763_nt++] = arr[i].o;
        }
    }
    free(arr);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A357763_cur == 0) {
        A357763_ext(1 << 16);
        A357763_addrec(1, 1);
        A357763_cur = 1;
        A357763_rebuild(1);
    }
    while (A357763_nt < (size_t)n) {
        if (A357763_cur >= ((uint64_t)1 << 62)) return 0;
        A357763_cur *= 2;
        A357763_stage(A357763_cur);
        A357763_rebuild(A357763_cur);
    }
    return (seq_t)A357763_terms[n - 1];
}
