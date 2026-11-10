/*
 * A349627  Numerators of the Möbius transform of ratio A003961(n)/sigma(n).
 * https://oeis.org/A349627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{d|n} mu(n/d)*A003961(d)/sigma(d);
 *            multiplicative, with f(p^e) = q^e/sigma(p^e) -
 *            q^(e-1)/sigma(p^(e-1)), q = nextprime(p)
 * Range:     u8 n <= 15, u16 n <= 242, u32 n <= 16383, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 113 terms through n = 9885 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static seq_t A349627_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

/* multiply running fraction num/den by the factor for prime power p^e */
static void A349627_apply(seq_t *num, seq_t *den, uint64_t p, int e) {
    seq_t q = (seq_t)next_prime(p);
    seq_t qe1 = 1;      /* q^(e-1) */
    seq_t s1 = 1;       /* sigma(p^(e-1)) */
    seq_t pw = 1;
    int i;
    for (i = 1; i < e; i++) {
        qe1 *= q;
        pw *= (seq_t)p;
        s1 += pw;
    }
    seq_t qe = qe1 * q;
    pw *= (seq_t)p;
    seq_t s2 = s1 + pw; /* sigma(p^e) */
    seq_t fn = qe * s1 - qe1 * s2;
    seq_t fd = s1 * s2;
    if (fn == 0) {
        *num = 0;
        *den = 1;
        return;
    }
    seq_t g = A349627_gcd(fn, fd);
    fn /= g;
    fd /= g;
    /* cross-reduce */
    g = A349627_gcd(*num, fd);
    if (g > 1) { *num /= g; fd /= g; }
    g = A349627_gcd(fn, *den);
    if (g > 1) { fn /= g; *den /= g; }
    *num *= fn;
    *den *= fd;
}

seq_t a(idx_t n) {
    seq_t num = 1, den = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            A349627_apply(&num, &den, p, e);
            if (num == 0) return 0;
        }
    }
    if (m > 1) {
        A349627_apply(&num, &den, m, 1);
    }
    return num;
}
