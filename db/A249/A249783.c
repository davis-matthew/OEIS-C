/*
 * A249783  Smallest index of a Fibonacci-like sequence containing n, see
 *          comments.
 * https://oeis.org/A249783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, nice
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for each k with u = Fib(k‑1) ≤ n
   find non‑negative solutions a*u + b*v = n where v = Fib(k)
   with u,v consecutive Fibonacci numbers co‑prime.
   The minimal value of a+b over all k gives a(n). */
#include <stdint.h>

static idx_t modinv(idx_t a, idx_t mod) {
    if (mod == 1) return 0;
    long long t = 0, newt = 1;
    long long r = mod, newr = a % mod;
    while (newr != 0) {
        long long q = r / newr;
        long long tmp = newt;
        newt = t - q * newt; t = tmp;
        tmp = newr;
        newr = r - q * newr; r = tmp;
    }
    if (r > 1) return 0;            // not invertible, should not happen
    if (t < 0) t += mod;
    return (idx_t)t;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          // trivial sequence 0,0,0,...
    idx_t best = n;               // k = 1 yields index n

    idx_t f0 = 0, f1 = 1;          // Fib(0), Fib(1)
    while (f0 <= n) {
        if (f0 != 0) {            // skip k = 1 (handled above)
            if (f1 == 1) {        // v = 1, sum is always n
                if (n < best) best = n;
            } else {
                idx_t inv = modinv(f0 % f1, f1);
                if (inv != 0) {
                    idx_t a0 = ((n % f1) * inv) % f1;
                    long long temp = (long long)n - (long long)a0 * (long long)f0;
                    if (temp >= 0) {
                        idx_t b0 = (idx_t)(temp / f1);
                        idx_t s = a0 + b0;
                        if (s < best) best = s;
                    }
                }
            }
        }
        // advance Fibonacci numbers
        idx_t f2 = f0 + f1;
        f0 = f1;
        f1 = f2;
    }
    return (seq_t)best;
}
