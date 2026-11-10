/*
 * A337789  Numbers k such that trajectory of k under repeated calculation of
 *          fecundity (x -> A070562(x)) eventually reaches 0.
 * https://oeis.org/A337789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n-th k >= 0 such that iterating A070562 from k reaches 0;
 *            search over k with cycle detection (Floyd)
 * Range:     u8 n <= 122, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9869 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A337789_fec(uint64_t x) {
    uint64_t steps = 0;
    while (1) {
        uint64_t p = 1, t = x;
        if (t == 0) break;
        while (t) { p *= t % 10; t /= 10; if (p == 0) break; }
        if (p == 0) break;
        x += p;
        steps++;
    }
    return steps;
}

static bool A337789_good(uint64_t k) {
    uint64_t slow = k, fast = k;
    if (k == 0) return true;
    while (1) {
        slow = A337789_fec(slow);
        if (slow == 0) return true;
        fast = A337789_fec(fast);
        if (fast == 0) return true;
        fast = A337789_fec(fast);
        if (fast == 0) return true;
        if (slow == fast) return false;
    }
}

seq_t a(idx_t n) {
    static idx_t last_n = 0;
    static uint64_t last_k = 0;
    idx_t cnt;
    uint64_t k;
    if (last_n != 0 && last_n <= n) {
        cnt = last_n;
        k = last_k;
    } else {
        cnt = 1;
        k = 0; /* a(1) = 0 */
    }
    while (cnt < n) {
        k++;
        if (A337789_good(k)) cnt++;
    }
    last_n = cnt;
    last_k = k;
    return (seq_t)k;
}
