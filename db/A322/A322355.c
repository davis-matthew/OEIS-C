/*
 * A322355  Lexicographically earliest such sequence a that a(i) = a(j) =>
 *          A322351(i) = A322351(j) and A322352(i) = A322352(j), for all i, j.
 * https://oeis.org/A322355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the unordered
 *            pair {A003557(n), A173557(n)}; computed by sorting the keys of
 *            1..n and counting first occurrences
 * Range:     u8 n <= 490, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 114 terms through n = 8432 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef struct { uint64_t lo, hi, idx; } A322355_item;

static int A322355_cmp(const void *x, const void *y) {
    const A322355_item *p = (const A322355_item *)x;
    const A322355_item *q = (const A322355_item *)y;
    if (p->lo != q->lo) return p->lo < q->lo ? -1 : 1;
    if (p->hi != q->hi) return p->hi < q->hi ? -1 : 1;
    if (p->idx != q->idx) return p->idx < q->idx ? -1 : 1;
    return 0;
}

static void A322355_pair(uint64_t m, uint64_t *lo, uint64_t *hi) {
    uint64_t sq = 1, ph = 1;
    for (uint64_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            ph *= (p - 1);
            for (int i = 1; i < e; i++) sq *= p;
        }
    }
    if (m > 1) ph *= (m - 1);
    if (sq <= ph) { *lo = sq; *hi = ph; } else { *lo = ph; *hi = sq; }
}

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    A322355_item *v = (A322355_item *)malloc(N * sizeof(A322355_item));
    for (size_t i = 0; i < N; i++) {
        v[i].idx = i + 1;
        A322355_pair(i + 1, &v[i].lo, &v[i].hi);
    }
    uint64_t nlo, nhi;
    nlo = 0; nhi = 0;
    for (size_t i = 0; i < N; i++)
        if (v[i].idx == (uint64_t)n) { nlo = v[i].lo; nhi = v[i].hi; }
    qsort(v, N, sizeof(A322355_item), A322355_cmp);
    unsigned char *first = (unsigned char *)calloc(N + 2, 1);
    uint64_t target = 0;
    for (size_t i = 0; i < N; i++) {
        if (i == 0 || v[i].lo != v[i - 1].lo || v[i].hi != v[i - 1].hi) {
            first[v[i].idx] = 1;
            if (v[i].lo == nlo && v[i].hi == nhi) target = v[i].idx;
        }
    }
    uint64_t cnt = 0;
    for (uint64_t i = 1; i <= target; i++) cnt += first[i];
    free(first);
    free(v);
    return (seq_t)cnt;
}
