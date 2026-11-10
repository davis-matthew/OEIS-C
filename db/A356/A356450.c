/*
 * A356450  Positions of numbers m = A005940(n+1) such that m < n.
 * https://oeis.org/A356450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th k such that A005940(k+1) < k; found
 *            by scanning k upward, with A005940(k+1) = f(k,1,1), f(m,i,x) = x
 *            if m=0, f(m/2,i+1,x) if m even, f((m-1)/2,i,x*prime(i)) if m odd
 * Range:     u8 n <= 26, u16 n <= 1394, u32 n <= 13175, u64 n <= 13175,
 *            u128 n <= 13175, big any n
 * Validated: standard, pass, 72 terms through n = 9470 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static void A356450_primes(uint64_t *p, int cnt) {
    int c = 0;
    uint64_t x = 2;
    while (c < cnt) {
        bool ok = true;
        for (uint64_t d = 2; d * d <= x; d++) {
            if (x % d == 0) { ok = false; break; }
        }
        if (ok) p[c++] = x;
        x++;
    }
}

seq_t a(idx_t n) {
    static uint64_t pr[80];
    static bool init = false;
    if (!init) {
        A356450_primes(pr, 80);
        init = true;
    }
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        k++;
        idx_t m = k;
        idx_t res = 1;
        int i = 1;
        bool ok = true;
        while (m > 0) {
            if (m & 1) {
                idx_t p = (idx_t)pr[i - 1];
                if (res > (k - 1) / p) { ok = false; break; }
                res *= p;
                m = (m - 1) / 2;
            } else {
                m /= 2;
                i++;
            }
        }
        if (ok && res < k) count++;
    }
    return (seq_t)k;
}
