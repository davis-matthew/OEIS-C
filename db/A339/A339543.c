/*
 * A339543  Beginnings of record-length chains of primes under iteration of
 *          A339541.
 * https://oeis.org/A339543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, bref, more
 * Formula:   search: a(n) is the n-th k >= 1 whose chain k, A339541(k),
 *            A339541(A339541(k)), ... begins with more primes than the chain
 *            of any smaller starting number
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A339543_isprime(uint64_t x) {
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= x; i += 6) {
        if (x % i == 0 || x % (i + 2) == 0) return false;
    }
    return true;
}

/* A339541(x) = x + sod(x, sod(x, 10)) */
static uint64_t A339541_f(uint64_t x) {
    uint64_t d = 0, t = x;
    while (t > 0) { d += t % 10; t /= 10; }
    if (d <= 1) return x + x;
    uint64_t s = 0;
    t = x;
    while (t > 0) { s += t % d; t /= d; }
    return x + s;
}

static int A339543_len(uint64_t x) {
    int c = 0;
    while (A339543_isprime(x)) {
        c++;
        x = A339541_f(x);
    }
    return c;
}

seq_t a(idx_t n) {
    static uint64_t rec[8];
    static int cnt = 0;
    static uint64_t cur = 0;
    static int best = -1;
    if (n < 1 || n > 5) return (seq_t)0; /* no further terms < 10^8 are known */
    while ((idx_t)cnt < n) {
        cur++;
        int L = A339543_len(cur);
        if (L > best) {
            best = L;
            rec[cnt++] = cur;
        }
    }
    return (seq_t)rec[n - 1];
}
