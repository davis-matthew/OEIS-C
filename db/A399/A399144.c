/*
 * A399144  Complement of A395718.
 * https://oeis.org/A399144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve: for v = 1, 2, ..., let m = floor(v/phi), L = v - m - 1, c
 *            = number of A395718 terms in (m, v); v is in A395718 iff c <=
 *            floor(L/phi). a(n) = n-th positive integer NOT admitted
 *            (complement of A395718)
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9117 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t A399144_isqrt(uint64_t x)
{
    uint64_t lo = 0, hi = 4294967295ULL, ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (mid * mid <= x) { ans = mid; lo = mid + 1; }
        else { if (mid == 0) break; hi = mid - 1; }
    }
    return ans;
}

/* floor(v/phi) = floor((floor(sqrt(20 v^2)) - 2v) / 4) */
static uint64_t A399144_fphi(uint64_t v)
{
    if (v == 0) return 0;
    uint64_t s = A399144_isqrt(20 * v * v);
    return (s - 2 * v) / 4;
}

seq_t a(idx_t n)
{
    static uint64_t cap = 0;
    static uint64_t *pre = NULL;     /* prefix counts of admitted values */
    static uint64_t *comp = NULL;    /* non-admitted values */
    static uint64_t ccount = 0;
    static uint64_t cur = 0;

    if (n == 0) return 0;
    if (pre == NULL) {
        cap = 1024;
        pre = (uint64_t *)malloc(cap * sizeof(uint64_t));
        comp = (uint64_t *)malloc(cap * sizeof(uint64_t));
        pre[0] = 0;
    }
    while (ccount < (uint64_t)n) {
        if (cur + 2 >= cap) {
            cap *= 2;
            pre = (uint64_t *)realloc(pre, cap * sizeof(uint64_t));
            comp = (uint64_t *)realloc(comp, cap * sizeof(uint64_t));
        }
        cur++;
        uint64_t v = cur;
        uint64_t m = A399144_fphi(v);
        uint64_t L = v - m - 1;
        uint64_t c = pre[v - 1] - pre[m];
        uint64_t rhs = A399144_fphi(L);
        int adm = (c <= rhs) ? 1 : 0;
        pre[v] = pre[v - 1] + (uint64_t)adm;
        if (!adm) comp[ccount++] = v;
    }
    return (seq_t)comp[n - 1];
}
