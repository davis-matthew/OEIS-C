/*
 * A289289  a(n) = A005259(n) mod (n+1)^3.
 * https://oeis.org/A289289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005259(n) mod (n+1)^3, where A005259(n) = Sum_{k=0..n}
 *            (C(n,k)*C(n+k,k))^2; t(k) = C(n,k)*C(n+k,k) is updated by t(k+1)
 *            = t(k)*(n-k)*(n+k+1)/(k+1)^2, done modulo (n+1)^3. Each t(k) is
 *            kept as a unit part times powers of the primes dividing n+1
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 1648, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9918 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A289289_mulmod(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

static uint64_t A289289_powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A289289_mulmod(r, b, m);
        b = A289289_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static uint64_t A289289_modinv(uint64_t a, uint64_t m) {
    /* a coprime to m */
    __int128 t = 0, nt = 1;
    __int128 r = (__int128)m, nr = (__int128)(a % m);
    while (nr != 0) {
        __int128 q = r / nr, tmp;
        tmp = t - q * nt; t = nt; nt = tmp;
        tmp = r - q * nr; r = nr; nr = tmp;
    }
    if (t < 0) t += (__int128)m;
    return (uint64_t)t;
}

static uint64_t A289289_strip(uint64_t x, const uint64_t *q, int nq, int *e, int sign) {
    for (int i = 0; i < nq; i++) {
        while (x % q[i] == 0) {
            x /= q[i];
            e[i] += sign;
        }
    }
    return x;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n + 1;
    uint64_t M = m * m * m;
    if (M == 1) return (seq_t)0;

    uint64_t q[24];
    int e[24];
    int nq = 0;
    uint64_t r = m;
    for (uint64_t p = 2; p * p <= r; p++) {
        if (r % p == 0) {
            q[nq] = p;
            e[nq++] = 0;
            while (r % p == 0) r /= p;
        }
    }
    if (r > 1) {
        q[nq] = r;
        e[nq++] = 0;
    }

    uint64_t unit = 1 % M;
    uint64_t total = 1 % M;
    for (uint64_t k = 0; k < (uint64_t)n; k++) {
        uint64_t x1 = A289289_strip((uint64_t)n - k, q, nq, e, 1);
        uint64_t x2 = A289289_strip((uint64_t)n + k + 1, q, nq, e, 1);
        uint64_t d = A289289_strip(k + 1, q, nq, e, -1);
        A289289_strip(k + 1, q, nq, e, -1); /* second factor (k+1) */
        uint64_t inv = A289289_modinv(d % M, M);
        unit = A289289_mulmod(unit, x1 % M, M);
        unit = A289289_mulmod(unit, x2 % M, M);
        unit = A289289_mulmod(unit, inv, M);
        unit = A289289_mulmod(unit, inv, M);

        uint64_t t = unit;
        for (int i = 0; i < nq; i++) {
            if (e[i] > 0) t = A289289_mulmod(t, A289289_powmod(q[i], (uint64_t)e[i], M), M);
        }
        total = (total + A289289_mulmod(t, t, M)) % M;
    }
    return (seq_t)total;
}
