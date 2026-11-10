/*
 * A276323  a(n) = (binomial(2 * prime(n + 3), prime(n + 3)) * A005259(prime(n
 *          + 3) - 1) - 2)/prime(n + 3)^5 for n >= 1.
 * https://oeis.org/A276323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (binomial(2*p, p) * A005259(p - 1) - 2)/p^5 with p =
 *            prime(n+3), where A005259(m) = Sum_{k=0..m}
 *            (binomial(m,k)*binomial(m+k,k))^2. For fixed-width seq_t the
 *            exact quotient is computed modulo 2^w (p^5 is odd, hence
 *            invertible), tracking 2-adic valuations so no intermediate
 *            overflows matter
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 23 terms through n = 79 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A276323_MUL(x, y) ((seq_t)((x) * ((y) + 0u)))

static bool A276323_isprime(uint64_t x) {
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A276323_nthprime(uint64_t m) {
    uint64_t c = 0, x = 1;
    while (c < m) {
        x++;
        if (A276323_isprime(x)) c++;
    }
    return x;
}

/* width in bits if seq_t wraps, 0 if arbitrary precision */
static unsigned A276323_width(void) {
    seq_t m = 1;
    unsigned w = 0;
    while (m != 0 && w < 300) {
        m = (seq_t)(m + m);
        w++;
    }
    return (m == 0) ? w : 0;
}

/* inverse of odd x modulo 2^w (wrapping arithmetic) */
static seq_t A276323_inv_odd(seq_t x) {
    seq_t y = x;
    for (int i = 0; i < 8; i++) {
        seq_t t = A276323_MUL(x, y);
        t = (seq_t)(2u - (t + 0u));
        y = A276323_MUL(y, t);
    }
    return y;
}

static unsigned A276323_ctz(uint64_t x) {
    unsigned c = 0;
    while ((x & 1) == 0) { x >>= 1; c++; }
    return c;
}

static void A276323_mul(seq_t *u, unsigned *v, uint64_t x) {
    unsigned c = A276323_ctz(x);
    *v += c;
    *u = A276323_MUL(*u, (seq_t)(x >> c));
}

static void A276323_div(seq_t *u, unsigned *v, uint64_t x) {
    unsigned c = A276323_ctz(x);
    *v -= c;
    *u = A276323_MUL(*u, A276323_inv_odd((seq_t)(x >> c)));
}

static seq_t A276323_value(seq_t u, unsigned v, unsigned w) {
    if (v >= w) return 0;
    seq_t p2 = 1;
    for (unsigned i = 0; i < v; i++) p2 = (seq_t)(p2 + p2);
    return A276323_MUL(u, p2);
}

static seq_t A005259_a(idx_t n) {
    seq_t t = 1;
    seq_t result = 1;
    for (idx_t k = 0; k < n; k++) {
        t = t * (seq_t)(n - k) / (seq_t)(k + 1);
        t = t * (seq_t)(n + k + 1) / (seq_t)(k + 1);
        result += t * t;
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t p = A276323_nthprime((uint64_t)n + 3);
    unsigned w = A276323_width();

    if (w == 0) {
        /* arbitrary precision: exact computation */
        seq_t c = 1;
        for (uint64_t i = 1; i <= p; i++)
            c = c * (seq_t)(p + i) / (seq_t)i;
        seq_t x = c * A005259_a((idx_t)(p - 1)) - 2;
        seq_t pp = (seq_t)p;
        seq_t p5 = pp * pp * pp * pp * pp;
        return x / p5;
    }

    /* fixed width: compute everything modulo 2^w */
    seq_t cu = 1;
    unsigned cv = 0;
    for (uint64_t i = 1; i <= p; i++) {
        A276323_mul(&cu, &cv, p + i);
        A276323_div(&cu, &cv, i);
    }
    seq_t C = A276323_value(cu, cv, w);

    uint64_t m = p - 1;
    seq_t tu = 1;
    unsigned tv = 0;
    seq_t A = 1;
    for (uint64_t k = 0; k < m; k++) {
        A276323_mul(&tu, &tv, m - k);
        A276323_div(&tu, &tv, k + 1);
        A276323_mul(&tu, &tv, m + k + 1);
        A276323_div(&tu, &tv, k + 1);
        seq_t t = A276323_value(tu, tv, w);
        A = (seq_t)(A + A276323_MUL(t, t));
    }

    seq_t x = A276323_MUL(C, A);
    x = (seq_t)(x - 2u);
    seq_t pp = (seq_t)p;
    seq_t p5 = pp;
    for (int i = 1; i < 5; i++) p5 = A276323_MUL(p5, pp);
    return A276323_MUL(x, A276323_inv_odd(p5));
}
