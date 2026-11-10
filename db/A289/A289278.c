/*
 * A289278  a(n) = A005259(n) mod (2*n+1)^2.
 * https://oeis.org/A289278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005259(n) mod (2*n+1)^2, A005259(n) = Sum_{k=0..n}
 *            (C(n,k)*C(n+k,k))^2; terms tracked mod M=(2n+1)^2 as (unit part
 *            mod M) * (powers of the primes dividing 2n+1)
 * Range:     u8 n <= 7, u16 n <= 121, u32 n <= 1031, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9930 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A289278_MAXP 16

typedef struct {
    idx_t ps[A289278_MAXP];
    int e[A289278_MAXP];
    int np;
    seq_t M;
    seq_t unit;
} A289278_st;

static seq_t A289278_inv(seq_t a, seq_t M) {
    seq_t r0 = M, r1 = a % M, t0 = 0, t1 = 1;
    while (r1 != 0) {
        seq_t q = r0 / r1;
        seq_t r2 = r0 - q * r1;
        seq_t t2 = (t0 + M - (q % M) * t1 % M) % M;
        r0 = r1; r1 = r2; t0 = t1; t1 = t2;
    }
    return t0 % M;
}

static void A289278_mul(A289278_st *s, idx_t x) {
    for (int i = 0; i < s->np; i++)
        while (x % s->ps[i] == 0) { x /= s->ps[i]; s->e[i]++; }
    s->unit = s->unit * ((seq_t)x % s->M) % s->M;
}

static void A289278_div(A289278_st *s, idx_t x) {
    for (int i = 0; i < s->np; i++)
        while (x % s->ps[i] == 0) { x /= s->ps[i]; s->e[i]--; }
    s->unit = s->unit * A289278_inv((seq_t)x % s->M, s->M) % s->M;
}

static seq_t A289278_val(const A289278_st *s) {
    seq_t v = s->unit;
    for (int i = 0; i < s->np; i++) {
        seq_t p = (seq_t)s->ps[i] % s->M;
        for (int j = 0; j < s->e[i]; j++) v = v * p % s->M;
    }
    return v;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t m = 2 * n + 1;
    A289278_st s;
    s.np = 0;
    idx_t r = m;
    for (idx_t p = 3; p * p <= r; p += 2) {
        if (r % p == 0) {
            s.ps[s.np] = p; s.e[s.np] = 0; s.np++;
            while (r % p == 0) r /= p;
        }
    }
    if (r > 1) { s.ps[s.np] = r; s.e[s.np] = 0; s.np++; }
    s.M = (seq_t)m * (seq_t)m;
    s.unit = 1 % s.M;
    seq_t result = 1 % s.M;
    for (idx_t k = 0; k < n; k++) {
        A289278_mul(&s, n - k);
        A289278_div(&s, k + 1);
        A289278_mul(&s, n + k + 1);
        A289278_div(&s, k + 1);
        seq_t t = A289278_val(&s);
        result = (result + t * t) % s.M;
    }
    return result;
}
