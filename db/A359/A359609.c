/*
 * A359609  Distinct values of A358777 in the order of their appearance.
 * https://oeis.org/A359609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A358777(A359608(n)). A358777 is 0 at even n > 1, and at
 *            odd n its value depends only on the prime signature: a(1) = 1
 *            and a(n) = -Sum_{d|n, d<n, bigomega(n/d) even} a(d). The code
 *            enumerates the prime signatures by the smallest odd number that
 *            has each one (3^e1*5^e2*..., e1>=e2>=...), ord...
 * Range:     i8 n <= 26, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

#define A359609_MAXP 40

typedef struct {
    unsigned __int128 N;
    unsigned char len;
    unsigned char e[A359609_MAXP];
} A359609_rec;

static unsigned A359609_pr[A359609_MAXP];
static int A359609_pr_ready = 0;

static A359609_rec *A359609_recs = NULL;
static size_t A359609_nrec = 0, A359609_caprec = 0;
static unsigned __int128 A359609_X = 0;

static seq_t *A359609_dist = NULL;
static size_t A359609_nd = 0;

static void A359609_init_primes(void)
{
    if (A359609_pr_ready) return;
    int k = 0;
    for (unsigned c = 3; k < A359609_MAXP; c += 2) {
        int ok = 1;
        for (unsigned d = 3; d * d <= c; d += 2)
            if (c % d == 0) { ok = 0; break; }
        if (ok) A359609_pr[k++] = c;
    }
    A359609_pr_ready = 1;
}

static void A359609_push(unsigned __int128 N, const unsigned char *e, int len)
{
    if (A359609_nrec >= A359609_caprec) {
        size_t nc = A359609_caprec ? A359609_caprec * 2 : 256;
        A359609_rec *t = (A359609_rec *)realloc(A359609_recs, nc * sizeof(A359609_rec));
        if (!t) exit(EXIT_FAILURE);
        A359609_recs = t;
        A359609_caprec = nc;
    }
    A359609_rec *r = &A359609_recs[A359609_nrec++];
    r->N = N;
    r->len = (unsigned char)len;
    for (int i = 0; i < A359609_MAXP; ++i) r->e[i] = (i < len) ? e[i] : 0;
}

static void A359609_dfs(int i, int maxe, unsigned __int128 N, unsigned char *e)
{
    A359609_push(N, e, i);
    if (i >= A359609_MAXP) return;
    unsigned p = A359609_pr[i];
    unsigned __int128 M = N;
    for (int ex = 1; ex <= maxe; ++ex) {
        if (M > A359609_X / p) break;
        M *= p;
        e[i] = (unsigned char)ex;
        A359609_dfs(i + 1, ex, M, e);
    }
    e[i] = 0;
}

static int A359609_cmp(const void *x, const void *y)
{
    const A359609_rec *a1 = (const A359609_rec *)x;
    const A359609_rec *b1 = (const A359609_rec *)y;
    if (a1->N < b1->N) return -1;
    if (a1->N > b1->N) return 1;
    return 0;
}

static size_t A359609_find(unsigned __int128 N)
{
    size_t lo = 0, hi = A359609_nrec;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (A359609_recs[mid].N < N) lo = mid + 1; else hi = mid;
    }
    return lo;
}

static void A359609_build(void)
{
    A359609_init_primes();
    A359609_nrec = 0;
    unsigned char e[A359609_MAXP];
    for (int i = 0; i < A359609_MAXP; ++i) e[i] = 0;
    A359609_dfs(0, 200, (unsigned __int128)1, e);
    qsort(A359609_recs, A359609_nrec, sizeof(A359609_rec), A359609_cmp);

    seq_t *vals = mem_new(seq_t, A359609_nrec);
    if (A359609_dist) mem_free(A359609_dist);
    A359609_dist = mem_new(seq_t, A359609_nrec + 2);
    A359609_nd = 0;
    int zero_added = 0;

    for (size_t r = 0; r < A359609_nrec; ++r) {
        const A359609_rec *R = &A359609_recs[r];
        int len = R->len;
        seq_t v;
        if (len == 0) {
            v = 1;
        } else {
            int tot = 0;
            for (int i = 0; i < len; ++i) tot += R->e[i];
            unsigned char sub[A359609_MAXP], srt[A359609_MAXP];
            for (int i = 0; i < len; ++i) sub[i] = 0;
            seq_t sum = 0;
            for (;;) {
                int st = 0, eq = 1;
                for (int i = 0; i < len; ++i) {
                    st += sub[i];
                    if (sub[i] != R->e[i]) eq = 0;
                }
                if (!eq && ((tot - st) & 1) == 0) {
                    int k = 0;
                    for (int i = 0; i < len; ++i) {
                        if (!sub[i]) continue;
                        unsigned char x = sub[i];
                        int j = k++;
                        while (j > 0 && srt[j - 1] < x) { srt[j] = srt[j - 1]; --j; }
                        srt[j] = x;
                    }
                    unsigned __int128 M = 1;
                    for (int i = 0; i < k; ++i)
                        for (int t = 0; t < srt[i]; ++t) M *= A359609_pr[i];
                    size_t idx = A359609_find(M);
                    sum += vals[idx];
                }
                int i = 0;
                while (i < len && sub[i] == R->e[i]) { sub[i] = 0; ++i; }
                if (i == len) break;
                ++sub[i];
            }
            v = -sum;
        }
        vals[r] = v;

        if (!zero_added && R->N > 2) {
            seq_t z = 0;
            int found = 0;
            for (size_t k = 0; k < A359609_nd; ++k)
                if (A359609_dist[k] == z) { found = 1; break; }
            if (!found) A359609_dist[A359609_nd++] = z;
            zero_added = 1;
        }
        int found = 0;
        for (size_t k = 0; k < A359609_nd; ++k)
            if (A359609_dist[k] == v) { found = 1; break; }
        if (!found) A359609_dist[A359609_nd++] = v;
    }
    if (!zero_added) {
        seq_t z = 0;
        int found = 0;
        for (size_t k = 0; k < A359609_nd; ++k)
            if (A359609_dist[k] == z) { found = 1; break; }
        if (!found) A359609_dist[A359609_nd++] = z;
    }
    mem_free(vals);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A359609_X == 0) {
        A359609_X = (unsigned __int128)1 << 16;
        A359609_build();
    }
    unsigned __int128 lim = (unsigned __int128)1 << 120;
    while (A359609_nd < (size_t)n && A359609_X < lim) {
        A359609_X <<= 4;
        A359609_build();
    }
    if ((size_t)n > A359609_nd) return 0;
    return A359609_dist[(size_t)n - 1];
}
