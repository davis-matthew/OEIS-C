/*
 * A353794  a(n) = A353791(sigma(A003961(n))), where A353791(n) = A003958(n) *
 *          A064989(n).
 * https://oeis.org/A353794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative with a(p^e) = F(1+q+...+q^e), q =
 *            nextprime(p), where F(m) = A003958(m)*A064989(m) is
 *            multiplicative: F(r^k) = ((r-1)*prevprime(r))^k for odd r,
 *            (r-1)^k = 1 for r = 2
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9162 (2026-11-10)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

static seq_t A353794_term(seq_t r) {
    seq_t t = r - 1;
    if (r > 2) {
        uint64_t p = (uint64_t)r - 1;
        while (!is_prime_u64(p)) p--;
        t = t * (seq_t)p;
    }
    return t;
}

static seq_t A353794_F(seq_t s) {
    seq_t res = 1;
    for (seq_t r = 2; r * r <= s; r += (r == 2 ? 1 : 2)) {
        if (s % r == 0) {
            seq_t term = A353794_term(r);
            while (s % r == 0) {
                s /= r;
                res *= term;
            }
        }
    }
    if (s > 1) res *= A353794_term(s);
    return res;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            seq_t q = (seq_t)next_prime(p);
            seq_t s = 1, pw = 1;
            for (idx_t i = 0; i < e; i++) { pw *= q; s += pw; }
            result *= A353794_F(s);
        }
    }
    if (m > 1) {
        seq_t q = (seq_t)next_prime(m);
        result *= A353794_F(1 + q);
    }
    return result;
}
