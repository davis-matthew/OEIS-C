/*
 * A278118  Irregular triangle T(n,k) = A278113(n,k) for 1 <= k <= A278116(n),
 *          read by rows.
 * https://oeis.org/A278118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   row n lists T(k)=floor(n*sqrt(2/prime(k))) for k=1,2,... while
 *            T(k)>=1 and T(k)^2*prime(k) is strictly less than
 *            T(k-1)^2*prime(k-1) (the descending run). The code finds the row
 *            by cumulative row lengths, then takes the k-th term
 * Range:     u8 n <= 798, u16 n <= 10104, u32 n <= 10104, u64 n <= 10104,
 *            u128 n <= 10104, big any n
 * Validated: standard, pass, 98 terms through n = 9010 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static uint64_t *A278118_pr = NULL;
static size_t A278118_pn = 0, A278118_pc = 0;

static uint64_t A278118_isqrt(uint64_t x)
{
    uint64_t r = (uint64_t)sqrt((double)x);
    while (r * r > x) r--;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

static uint64_t A278118_prime(size_t k) /* k is 0-based */
{
    while (A278118_pn <= k) {
        uint64_t c;
        if (A278118_pn == 0) c = 2;
        else if (A278118_pn == 1) c = 3;
        else {
            c = A278118_pr[A278118_pn - 1] + 2;
            for (;; c += 2) {
                bool ok = true;
                for (size_t i = 1; i < A278118_pn && A278118_pr[i] * A278118_pr[i] <= c; i++)
                    if (c % A278118_pr[i] == 0) { ok = false; break; }
                if (ok) break;
            }
        }
        if (A278118_pn >= A278118_pc) {
            A278118_pc = A278118_pc ? A278118_pc * 2 : 64;
            A278118_pr = (uint64_t *)realloc(A278118_pr, A278118_pc * sizeof(uint64_t));
        }
        A278118_pr[A278118_pn++] = c;
    }
    return A278118_pr[k];
}

/* term k (1-based) of row r, or 0 if the row has ended before k */
static uint64_t A278118_row_len(uint64_t r, uint64_t want, uint64_t *out)
{
    uint64_t prev = 2 * r * r;
    uint64_t cnt = 1;
    if (want == 1) *out = r;
    for (size_t k = 1;; k++) {
        uint64_t p = A278118_prime(k);
        uint64_t t = A278118_isqrt((2 * r * r) / p);
        if (t == 0) break;
        uint64_t v = t * t * p;
        if (v >= prev) break;
        prev = v;
        cnt++;
        if (cnt == want) *out = t;
    }
    return cnt;
}

static uint64_t *A278118_cum = NULL;
static size_t A278118_cl = 0, A278118_cc = 0;

seq_t a(idx_t n)
{
    uint64_t dummy = 0;
    if (n == 0) return 0;
    while (A278118_cl == 0 || A278118_cum[A278118_cl - 1] < (uint64_t)n) {
        uint64_t r = (uint64_t)A278118_cl + 1;
        uint64_t len = A278118_row_len(r, 0, &dummy);
        if (A278118_cl >= A278118_cc) {
            A278118_cc = A278118_cc ? A278118_cc * 2 : 64;
            A278118_cum = (uint64_t *)realloc(A278118_cum, A278118_cc * sizeof(uint64_t));
        }
        A278118_cum[A278118_cl] = (A278118_cl ? A278118_cum[A278118_cl - 1] : 0) + len;
        A278118_cl++;
    }
    size_t lo = 0, hi = A278118_cl;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (A278118_cum[mid] >= (uint64_t)n) hi = mid; else lo = mid + 1;
    }
    uint64_t prev = lo ? A278118_cum[lo - 1] : 0;
    uint64_t k = (uint64_t)n - prev;
    uint64_t out = 0;
    A278118_row_len((uint64_t)lo + 1, k, &out);
    return (seq_t)out;
}
