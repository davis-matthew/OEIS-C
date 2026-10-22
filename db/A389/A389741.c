/*
 * A389741  Number of integer compositions of n whose parts are not in
 *          arithmetic progression.
 * https://oeis.org/A389741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) - (number of compositions of n that are
 *            arithmetic progressions). The arithmetic compositions are
 *            counted by iterating over the number of parts k up to n and
 *            enumerating all integer common differences d that satisfy the
 *            sum constraint and the positivity bounds
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   gcd64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/gcd64.h"

/* 64‑bit absolute GCD */

/* Extended Euclidean algorithm: returns gcd and sets x, y */
static uint64_t egcd(int64_t a, int64_t b, int64_t *x, int64_t *y) {
    int64_t x0 = 1, y0 = 0, x1 = 0, y1 = 1;
    while (b != 0) {
        int64_t q = a / b;
        int64_t tmp = a - q * b;  a = b;  b = tmp;
        tmp = x0 - q * x1;  x0 = x1;  x1 = tmp;
        tmp = y0 - q * y1;  y0 = y1;  y1 = tmp;
    }
    *x = x0; *y = y0;
    return a;
}

/* Modular inverse of a modulo m, where gcd(a,m)=1. Returns -1 if not invertible. */
static int64_t modinv(int64_t a, int64_t m) {
    int64_t x, y;
    int64_t g = egcd(a, m, &x, &y);
    if (g != 1) return -1;
    x %= m;
    if (x < 0) x += m;
    return x;
}

/* floor division for signed 64‑bit numbers, divisor > 0 */
static int64_t floor_div(int64_t a, int64_t b) {
    int64_t q = a / b;
    int64_t r = a % b;
    if (r != 0 && ((a < 0) != (b < 0))) q -= 1;
    return q;
}

/* ceil division for signed 64‑bit numbers, divisor > 0 */
static int64_t ceil_div(int64_t a, int64_t b) {
    int64_t q = a / b;
    int64_t r = a % b;
    if (r != 0 && ((a > 0) == (b > 0))) q += 1;
    return q;
}

seq_t a(idx_t n) {
    /* Base case */
    if (n == 0) return 0;

    /* Total number of compositions: 2^(n-1) */
    seq_t total = pow2(n - 1);

    /* Count arithmetic‐progression compositions */
    seq_t apcnt = 0;

    /* k = 1 contributes exactly one composition (the single part [n]) */
    if (n > 0) apcnt += (seq_t)1;

    /* k ≥ 2 */
    uint64_t un = (uint64_t)n;
    for (uint64_t k = 2; k <= un; ++k) {
        uint64_t s = k * (k - 1) / 2;        /* sum coefficient */
        uint64_t g = gcd64(s, k);
        if (un % g != 0) continue;          /* no integer solution for d */

        uint64_t kp = k / g;                 /* modulus for d */
        uint64_t sp = s / g;
        /* Inverse of sp modulo kp */
        int64_t inv_sp = modinv((int64_t)(sp % kp), (int64_t)kp);
        if (inv_sp < 0) continue;           /* should not happen */

        /* particular solution d0 modulo kp */
        uint64_t d0 = ((un / g) % kp) * (uint64_t)inv_sp % kp;
        /* bounds for d: (k-n)/s ≤ d ≤ (n-k)/s   (s>0) */
        int64_t L = ceil_div((int64_t)(k) - (int64_t)(un), (int64_t)s);
        int64_t U = floor_div((int64_t)(un) - (int64_t)(k), (int64_t)s);

        /* Translate bounds to t such that d = d0 + t*kp */
        int64_t first_t = ceil_div(L - (int64_t)d0, (int64_t)kp);
        int64_t last_t  = floor_div(U - (int64_t)d0, (int64_t)kp);
        if (last_t >= first_t) {
            uint64_t cnt = (uint64_t)(last_t - first_t + 1);
            apcnt += (seq_t)cnt;
        }
    }

    return total - apcnt;
}
