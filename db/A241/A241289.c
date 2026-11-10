/*
 * A241289  Numbers n for which in the factorization of n! over distinct terms
 *          of A050376, the numbers of prime and nonprime terms are equal.
 * https://oeis.org/A241289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th m >= 2 such that, writing the exponent e_p of each
 *            prime p in m! in binary, #{p : e_p odd} = sum over p of
 *            (popcount(e_p) - (e_p mod 2)); done by direct search (sequence
 *            is finite, 9 terms, last is 71)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int A241289_balanced(int m) {
    int primes = 0, nonprimes = 0;
    for (int p = 2; p <= m; p++) {
        bool ok = true;
        for (int d = 2; d * d <= p; d++) {
            if (p % d == 0) { ok = false; break; }
        }
        if (!ok) continue;
        long e = 0;
        long q = m;
        while (q > 0) { q /= p; e += q; }
        if (e & 1) primes++;
        e >>= 1;
        while (e > 0) { nonprimes += (int)(e & 1); e >>= 1; }
    }
    return primes == nonprimes;
}

seq_t a(idx_t n) {
    static int built = 0;
    static int terms[64];
    static int cnt = 0;
    if (!built) {
        for (int m = 2; m <= 300; m++) {
            if (A241289_balanced(m)) {
                if (cnt < 64) terms[cnt++] = m;
            }
        }
        built = 1;
    }
    if (n < 1 || n > (idx_t)cnt) return (seq_t)0;
    return (seq_t)terms[(int)(n - 1)];
}
