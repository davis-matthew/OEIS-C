/*
 * A364499  a(n) = A005940(n) - n.
 * https://oeis.org/A364499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A005940(n) - n, where A005940(n) = f(n-1, 1, 1) with f(0,
 *            i, x) = x, f(2m, i, x) = f(m, i+1, x), f(2m+1, i, x) = f(m, i,
 *            x*prime(i))
 * Range:     i8 n <= 44, i16 n <= 440, i32 n <= 8176, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 97 terms through n = 9015 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A364499_prime_at(int k) {
    static uint64_t pr[72];
    static int ready = 0;
    if (!ready) {
        int c = 0;
        uint64_t x = 2;
        while (c < 72) {
            bool ok = true;
            for (int j = 0; j < c && pr[j] * pr[j] <= x; j++) {
                if (x % pr[j] == 0) { ok = false; break; }
            }
            if (ok) pr[c++] = x;
            x++;
        }
        ready = 1;
    }
    return pr[k - 1];
}

seq_t a(idx_t n) {
    idx_t m = n - 1;
    seq_t result = 1;
    int i = 1;
    while (m > 0) {
        if (m & 1) {
            result *= (seq_t)A364499_prime_at(i);
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }
    return result - (seq_t)n;
}
