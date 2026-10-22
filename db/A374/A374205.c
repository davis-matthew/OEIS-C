/*
 * A374205  The 5-adic valuation of A328845(n), where A328845 is a
 *          Fibonacci-based variant of the arithmetic derivative.
 * https://oeis.org/A374205
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 30, u32 n <= 46, u64 n <= 8466,
 *            u128 n <= 8466, big any n
 * Helpers:   fib_mod_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = A112765(A328845(n)), the 5‑adic valuation of
 * A328845(n).  A328845(n) = Σ_{p^e∥n} e * F(p) * (n/p),
 * where F(k) is the k‑th Fibonacci number with F(1)=F(2)=1.
 * The implementation uses modular arithmetic modulo 5^15 to avoid
 * overflow and works for any seq_t (64‑bit unsigned or arbitrary‑precision).
 * Prime factorisation of n is done by trial division. */

#include <stdint.h>
#include "../utils/fib_mod_n.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* offset = 2 */

    /* modulus 5^15 – more than enough for n ≤ 10000 */
    seq_t mod = 1;
    for (int i = 0; i < 15; ++i) mod *= 5u;

    seq_t sum = 0;
    idx_t orig = n;

    /* factor 2 first */
    int e2 = 0;
    while ((n & 1) == 0) { n >>= 1; ++e2; }

    if (e2 > 0) {
        seq_t term = (seq_t)e2 * (seq_t)(orig / 2);  /* F(2)=1 */
        term %= mod;
        sum += term;
        if (sum >= mod) sum -= mod;
    }

    /* odd prime factors */
    idx_t m = n;
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            seq_t fibp = fib_mod_n(p, mod);
            seq_t term = (seq_t)e * fibp * (seq_t)(orig / p);
            term %= mod;
            sum += term;
            if (sum >= mod) sum -= mod;
        }
    }

    if (m > 1) {                       /* remaining prime factor */
        seq_t fibm = fib_mod_n(m, mod);
        seq_t term = fibm * (seq_t)(orig / m);
        term %= mod;
        sum += term;
        if (sum >= mod) sum -= mod;
    }

    /* 5‑adic valuation of sum */
    int v = 0;
    seq_t tmp = sum;
    while (tmp > 0 && tmp % 5 == 0) {
        tmp /= 5;
        ++v;
    }
    return (seq_t)v;
}
