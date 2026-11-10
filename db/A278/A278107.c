/*
 * A278107  a(n) is the first k such that A277515(k) is the n-th prime.
 * https://oeis.org/A278107
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first k such that A277515(k) = prime(n), where A277515(k)
 *            is the smallest odd prime p with m^2*p strictly between 2k^2 and
 *            2(k+1)^2 for some integer m. Computed by a linear search over k
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 62 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A278107_MAXP 1000

static uint64_t A278107_isqrt(uint64_t x)
{
    uint64_t r = 0;
    uint64_t lo = 0, hi = 4294967296ULL;
    (void)r;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= x) lo = mid; else hi = mid - 1;
    }
    return lo;
}

static uint64_t A278107_found[A278107_MAXP];
static int A278107_np = 0;
static int A278107_pr[A278107_MAXP];
static uint64_t A278107_k = 0;   /* last k scanned */
static bool A278107_init = false;

static void A278107_setup(void)
{
    int i, j;
    A278107_init = true;
    A278107_np = 0;
    for (i = 2; i < A278107_MAXP; i++) {
        bool pr = true;
        for (j = 2; j * j <= i; j++)
            if (i % j == 0) { pr = false; break; }
        if (pr) A278107_pr[A278107_np++] = i;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    if (!A278107_init) A278107_setup();
    if (n > (idx_t)A278107_np) return (seq_t)0;
    while (A278107_found[n - 1] == 0) {
        uint64_t k = ++A278107_k;
        uint64_t lim = 2 * (k + 1) * (k + 1);
        uint64_t base = 2 * k * k;
        int i;
        for (i = 1; i < A278107_np; i++) {
            uint64_t p = (uint64_t)A278107_pr[i];
            uint64_t m = A278107_isqrt(base / p) + 1;
            if (m * m * p < lim) {
                if (A278107_found[i] == 0) A278107_found[i] = k;
                break;
            }
        }
    }
    return (seq_t)A278107_found[n - 1];
}
