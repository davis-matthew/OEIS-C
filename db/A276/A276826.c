/*
 * A276826  a(n) is the maximal difference between the corresponding terms of
 *          sequences defined in the same way as A159559, but with initial
 *          terms A001359(n-1)+2 and A001359(n-1) respectively.
 * https://oeis.org/A276826
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max over k >= 2 of |u(k) - v(k)|, where u and v are the
 *            A159559 recurrence (u(k) = least m > u(k-1) with m prime iff k
 *            prime) started with u(2) = A001359(n-1)+2 and v(2) =
 *            A001359(n-1). Computed by simulating both until they coincide
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 68 (2026-11-08)
 * Helpers:   is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_u64.h"

static bool A276826_is_prime_big(seq_t n) {
    if (n < 2) return false;
    if (sizeof(seq_t) <= 8 || !((seq_t)UINT64_MAX < n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    static const int bases[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (int i = 0; i < 12; i++) {
        seq_t a = (seq_t)bases[i];
        if (a >= n) continue;
        seq_t x = 1, pw = a % n, e = d;
        while (e > 0) {
            if (e & 1) x = (x * pw) % n;
            pw = (pw * pw) % n;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t A276826_next(seq_t cur, bool need_prime) {
    cur = cur + 1;
    while (A276826_is_prime_big(cur) != need_prime) cur = cur + 1;
    return cur;
}

seq_t a(idx_t n) {
    /* (n-1)-th lesser twin prime */
    idx_t cnt = 0, p = 2;
    for (;; p++) {
        if (is_prime(p) && is_prime(p + 2)) {
            cnt++;
            if (cnt == n - 1) break;
        }
    }
    seq_t x = (seq_t)p + 2, y = (seq_t)p, best = 0;
    idx_t k = 2;
    while (x != y) {
        seq_t diff = x > y ? x - y : y - x;
        if (diff > best) best = diff;
        k++;
        bool pr = is_prime(k);
        x = A276826_next(x, pr);
        y = A276826_next(y, pr);
    }
    return best;
}
