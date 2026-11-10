/*
 * A289172  Irregular triangle read by rows: row n lists terms m of A038566(n)
 *          such that A001221(m) = A051265(n), with a(1) = 1.
 * https://oeis.org/A289172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = n-th entry of the triangle whose row r lists the m in
 *            1..r with gcd(m,r)=1 and omega(m) = max omega over such m
 *            (A051265(r)); computed by sieve and row scan
 * Range:     u8 n <= 2154, u16 n <= 15549, u32 n <= 15549, u64 n <= 15549,
 *            u128 n <= 15549, big any n
 * Validated: standard, pass, 90 terms through n = 9776 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned char *A289172_om = NULL;
static size_t A289172_omcap = 0;
static uint64_t *A289172_cum = NULL;   /* cum[r] = total terms in rows 1..r */
static size_t A289172_rows = 0, A289172_cumcap = 0;

static void A289172_ensure_omega(size_t lim)
{
    if (lim < A289172_omcap) return;
    size_t cap = A289172_omcap ? A289172_omcap : 1024;
    while (cap <= lim) cap *= 2;
    free(A289172_om);
    A289172_om = (unsigned char *)calloc(cap + 1, 1);
    if (!A289172_om) exit(1);
    A289172_omcap = cap;
    for (size_t p = 2; p <= cap; ++p)
        if (A289172_om[p] == 0)
            for (size_t m = p; m <= cap; m += p) A289172_om[m]++;
}

static uint64_t A289172_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static uint64_t A289172_rowcount(uint64_t r)
{
    A289172_ensure_omega((size_t)r);
    int mx = -1;
    uint64_t cnt = 0;
    for (uint64_t k = 1; k <= r; ++k) {
        if (A289172_gcd(k, r) != 1) continue;
        int w = A289172_om[k];
        if (w > mx) { mx = w; cnt = 1; }
        else if (w == mx) cnt++;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (A289172_cum == NULL) {
        A289172_cumcap = 1024;
        A289172_cum = (uint64_t *)malloc(A289172_cumcap * sizeof(uint64_t));
        if (!A289172_cum) exit(1);
        A289172_cum[0] = 0;
        A289172_rows = 0;
    }
    while (A289172_cum[A289172_rows] < N) {
        uint64_t r = A289172_rows + 1;
        if (r + 1 >= A289172_cumcap) {
            A289172_cumcap *= 2;
            A289172_cum = (uint64_t *)realloc(A289172_cum, A289172_cumcap * sizeof(uint64_t));
            if (!A289172_cum) exit(1);
        }
        A289172_cum[r] = A289172_cum[r - 1] + A289172_rowcount(r);
        A289172_rows = r;
    }
    /* binary search smallest r with cum[r] >= N */
    size_t lo = 1, hi = A289172_rows;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (A289172_cum[mid] >= N) hi = mid; else lo = mid + 1;
    }
    uint64_t r = lo;
    uint64_t k = N - A289172_cum[r - 1];
    A289172_ensure_omega((size_t)r);
    int mx = -1;
    for (uint64_t m = 1; m <= r; ++m)
        if (A289172_gcd(m, r) == 1 && A289172_om[m] > mx) mx = A289172_om[m];
    uint64_t c = 0;
    for (uint64_t m = 1; m <= r; ++m) {
        if (A289172_gcd(m, r) == 1 && A289172_om[m] == mx) {
            if (++c == k) return (seq_t)m;
        }
    }
    return (seq_t)0;
}
