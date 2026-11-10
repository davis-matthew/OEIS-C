/*
 * A300157  Records in A299990.
 * https://oeis.org/A300157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n-th record value of A299990(k) = A010846(k) -
 *            2*A000005(k), where A010846(k) = #{j <= k : every prime of j
 *            divides k}. Computed by a search over the prime sets S =
 *            primes(k), in doubling windows (lo, 2*lo]. A prime set is
 *            skipped when an upper bound on its smooth-number count is no
 *            larger...
 * Range:     i8 n <= 22, i16 n <= 108, i32 n <= 108, i64 n <= 108,
 *            i128 n <= 108, big any n
 * Validated: standard, pass, 70 terms through n = 108 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A_MAXP 16

static uint32_t A_sp[6600];
static int A_nsp = 0;
static bool A_inited = false;

static void A_sieve(void) {
    static unsigned char comp[65537];
    for (uint32_t i = 2; i <= 65536; ++i) {
        if (!comp[i]) {
            A_sp[A_nsp++] = i;
            for (uint32_t j = i * 2; j <= 65536; j += i) comp[j] = 1;
        }
    }
    A_inited = true;
}

static bool A_isprime(uint64_t c) {
    if (c < 2) return false;
    for (int t = 0; t < A_nsp; ++t) {
        uint64_t p = A_sp[t];
        if (p * p > c) return true;
        if (c % p == 0) return c == p;
    }
    return true;
}

static uint64_t A_nextp(uint64_t p) {
    uint64_t c = p + 1;
    while (!A_isprime(c)) ++c;
    return c;
}

static int A_bitlen(uint64_t x) {
    int b = 0;
    while (x) { ++b; x >>= 1; }
    return b;
}

/* q[] descending; counts j <= x built only from primes q[i..m-1] */
static unsigned long long A_psi_rec(int i, int m, const uint64_t *q, uint64_t x) {
    uint64_t p = q[i];
    if (i == m - 1) {
        if (p == 2) return (unsigned long long)A_bitlen(x);
        unsigned long long c = 0;
        while (x) { ++c; x /= p; }
        return c;
    }
    unsigned long long c = 0;
    while (x) {
        c += A_psi_rec(i + 1, m, q, x);
        x /= p;
    }
    return c;
}

static unsigned long long A_psi(const uint64_t *asc, int m, uint64_t x) {
    uint64_t q[A_MAXP];
    if (m == 0) return x >= 1 ? 1ULL : 0ULL;
    for (int j = 0; j < m; ++j) q[j] = asc[m - 1 - j];
    return A_psi_rec(0, m, q, x);
}

/* segment state */
static uint64_t A_N, A_lo;
static long long A_T;

typedef struct { uint64_t n; long long f; } A_cand;
static A_cand *A_cands = NULL;
static size_t A_ncand = 0, A_capcand = 0;

static void A_add(uint64_t n, long long f) {
    if (A_ncand == A_capcand) {
        size_t nc = A_capcand ? A_capcand * 2 : 256;
        A_cands = (A_cand *)realloc(A_cands, nc * sizeof(A_cand));
        A_capcand = nc;
    }
    A_cands[A_ncand].n = n;
    A_cands[A_ncand].f = f;
    ++A_ncand;
}

static int A_cmp(const void *x, const void *y) {
    const A_cand *u = (const A_cand *)x, *v = (const A_cand *)y;
    return (u->n > v->n) - (u->n < v->n);
}

/* enumeration of n = r*t with rad(n) = r */
static uint64_t A_cur_r, A_cur_lim;
static uint64_t A_cur_S[A_MAXP], A_cur_q[A_MAXP];
static int A_cur_m;
static long long A_cur_psiN;

static void A_enum(int idx, uint64_t t, long long d) {
    if (idx == A_cur_m) {
        uint64_t n = A_cur_r * t;
        if (n <= A_lo) return;
        if (A_cur_psiN - 2 * d <= A_T) return;
        long long c = (long long)A_psi_rec(0, A_cur_m, A_cur_q, n);
        long long f = c - 2 * d;
        if (f > A_T) A_add(n, f);
        return;
    }
    uint64_t p = A_cur_S[idx];
    uint64_t v = t;
    long long e = 0;
    for (;;) {
        A_enum(idx + 1, v, d * (e + 2));
        if (v > A_cur_lim / p) break;
        v *= p;
        ++e;
    }
}

static void A_process(const uint64_t *S, int m, uint64_t prod) {
    long long psiN = (long long)A_psi(S, m, A_N);
    if (psiN <= A_T) return;
    for (int j = 0; j < m; ++j) {
        A_cur_S[j] = S[j];
        A_cur_q[j] = S[m - 1 - j];
    }
    A_cur_m = m;
    A_cur_r = prod;
    A_cur_lim = A_N / prod;
    A_cur_psiN = psiN;
    A_enum(0, 1, 1);
}

static void A_dfs(uint64_t *S, int m, uint64_t prod, uint64_t p) {
    for (;; p = A_nextp(p)) {
        if (prod > A_N / p) break;
        /* bound: S + {p} + the following primes, as many as fit in the product limit */
        uint64_t arr[A_MAXP];
        for (int j = 0; j < m; ++j) arr[j] = S[j];
        arr[m] = p;
        int cnt = m + 1;
        uint64_t pr = prod * p, e = p;
        while (cnt < A_MAXP) {
            e = A_nextp(e);
            if (pr > A_N / e) break;
            pr *= e;
            arr[cnt++] = e;
        }
        long long bound = (long long)A_psi(arr, cnt, A_N);
        if (bound <= A_T) break;   /* monotone in p: larger p only smaller */
        S[m] = p;
        A_process(S, m + 1, prod * p);
        A_dfs(S, m + 1, prod * p, A_nextp(p));
    }
}

static long long *A_recs = NULL;
static size_t A_nrec = 0, A_caprec = 0;
static long long A_best;
static uint64_t A_hi = 0;

static void A_push_rec(long long v) {
    if (A_nrec == A_caprec) {
        size_t nc = A_caprec ? A_caprec * 2 : 256;
        A_recs = (long long *)realloc(A_recs, nc * sizeof(long long));
        A_caprec = nc;
    }
    A_recs[A_nrec++] = v;
}

static void A_segment(void) {
    A_lo = A_hi;
    A_N = A_hi * 2;
    A_T = A_best;
    A_ncand = 0;
    uint64_t S[A_MAXP];
    A_dfs(S, 0, 1, 2);
    if (A_ncand > 1) qsort(A_cands, A_ncand, sizeof(A_cand), A_cmp);
    for (size_t i = 0; i < A_ncand; ++i) {
        if (A_cands[i].f > A_best) {
            A_best = A_cands[i].f;
            A_push_rec(A_best);
        }
    }
    A_hi = A_N;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (!A_inited) A_sieve();
    if (A_hi == 0) {
        A_best = -1;           /* A299990(1) = -1 */
        A_push_rec(-1);
        A_hi = 1;
    }
    while (A_nrec < (size_t)n) A_segment();
    return (seq_t)A_recs[n - 1];
}
