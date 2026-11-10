/*
 * A323366  Lexicographically earliest sequence such that for all i, j, a(i) =
 *          a(j) => A000035(i) = A000035(j) and A003557(i) = A003557(j) and
 *          A173557(i) = A173557(j).
 * https://oeis.org/A323366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   restricted-growth labelling (search): a(n) = 1 + number of
 *            distinct keys (A000035(k), A003557(k), A173557(k)) whose first
 *            occurrence k is below the first occurrence of n's key; computed
 *            by sorting the keys of 1..n
 * Range:     u8 n <= 310, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9966 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint64_t b, c, idx;
    unsigned par;
} A323366_rec;

static int A323366_keycmp(const A323366_rec *p, const A323366_rec *q)
{
    if (p->par != q->par) return p->par < q->par ? -1 : 1;
    if (p->b != q->b) return p->b < q->b ? -1 : 1;
    if (p->c != q->c) return p->c < q->c ? -1 : 1;
    return 0;
}

static int A323366_cmp(const void *x, const void *y)
{
    const A323366_rec *p = (const A323366_rec *)x;
    const A323366_rec *q = (const A323366_rec *)y;
    int k = A323366_keycmp(p, q);
    if (k) return k;
    if (p->idx != q->idx) return p->idx < q->idx ? -1 : 1;
    return 0;
}

static void A323366_key(uint64_t n, A323366_rec *r)
{
    uint64_t b = 1, c = 1, m = n;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            c *= (p - 1);
            for (int i = 1; i < e; i++) b *= p;
        }
    }
    if (m > 1) c *= (m - 1);
    r->b = b; r->c = c; r->par = (unsigned)(n & 1u); r->idx = n;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    A323366_rec *v = (A323366_rec *)malloc(N * sizeof(A323366_rec));
    unsigned char *first = (unsigned char *)calloc(N + 1, 1);
    size_t i;
    uint64_t f = (uint64_t)n;
    for (i = 0; i < N; i++) A323366_key((uint64_t)(i + 1), &v[i]);
    qsort(v, N, sizeof(A323366_rec), A323366_cmp);
    size_t gstart = 0;
    for (i = 0; i < N; i++) {
        if (i == 0 || A323366_keycmp(&v[i], &v[i - 1]) != 0) {
            gstart = i;
            first[v[i].idx] = 1;
        }
        if (v[i].idx == (uint64_t)n) f = v[gstart].idx;
    }
    uint64_t cnt = 0;
    for (uint64_t m = 1; m <= f; m++) cnt += first[m];
    free(v);
    free(first);
    return (seq_t)cnt;
}
