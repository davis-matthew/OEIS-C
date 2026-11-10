/*
 * A375493  "s" coefficients of a log_2-like sequence of algebraic integers of
 *          the form (r+phi*s), where r and s are integers and phi =
 *          (1+sqrt(5))/2 is the golden ratio.
 * https://oeis.org/A375493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, changed
 * Formula:   a(n) = b(n) where f(n) = a(n)+phi*b(n) is totally additive,
 *            f(1)=0, f(2)=1, and for odd primes p: f(p) =
 *            (f(p-1)+phi*f(p+1))/(1+phi); this is the phi-coefficient
 * Range:     i8 n <= 562, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9175 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

#define A375493_LIM 200000

static seq_t *A375493_ca = NULL;
static seq_t *A375493_cb = NULL;
static unsigned char *A375493_cf = NULL;

static bool A375493_isprime(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

static void A375493_f(idx_t n, seq_t *ra, seq_t *rb)
{
    if (n <= 1) { *ra = 0; *rb = 0; return; }
    if (n == 2) { *ra = 1; *rb = 0; return; }
    if (A375493_isprime(n)) {
        if (n < A375493_LIM && A375493_cf[n]) {
            *ra = A375493_ca[n]; *rb = A375493_cb[n];
            return;
        }
        seq_t a1, b1, a2, b2;
        A375493_f(n - 1, &a1, &b1);
        A375493_f(n + 1, &a2, &b2);
        seq_t c = a1 + b2;
        seq_t d = b1 + a2 + b2;
        seq_t ra_ = 2 * c - d;
        seq_t rb_ = d - c;
        if (n < A375493_LIM) {
            A375493_ca[n] = ra_;
            A375493_cb[n] = rb_;
            A375493_cf[n] = 1;
        }
        *ra = ra_; *rb = rb_;
        return;
    }
    seq_t sa = 0, sb = 0;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            long long e = 0;
            while (t % p == 0) { t /= p; ++e; }
            seq_t pa, pb;
            A375493_f(p, &pa, &pb);
            sa += (seq_t)e * pa;
            sb += (seq_t)e * pb;
        }
    }
    if (t > 1) {
        seq_t pa, pb;
        A375493_f(t, &pa, &pb);
        sa += pa;
        sb += pb;
    }
    *ra = sa; *rb = sb;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A375493_cf == NULL) {
        A375493_ca = mem_new(seq_t, A375493_LIM);
        A375493_cb = mem_new(seq_t, A375493_LIM);
        A375493_cf = (unsigned char *)calloc(A375493_LIM, 1);
    }
    seq_t ra, rb;
    A375493_f(n, &ra, &rb);
    return rb;
}
