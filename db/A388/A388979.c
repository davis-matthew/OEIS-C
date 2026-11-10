/*
 * A388979  a(n) = A326127(n) * A389078(n).
 * https://oeis.org/A388979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A326127(n)*A389078(n) = (sigma(n) - s - n)*(A003961(n) -
 *            A003961(s)), where s = A007913(n) is the squarefree part of n
 * Range:     u8 n <= 11, u16 n <= 71, u32 n <= 5183, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 92 terms through n = 7935 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    seq_t sigma = 1, s = 1, a3n = 1, a3s = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p != 0) continue;
        uint64_t q = next_prime(p);
        seq_t pw = 1, sum = 1;
        int e = 0;
        while (m % p == 0) {
            m /= p;
            e++;
            pw *= (seq_t)p;
            sum += pw;
            a3n *= (seq_t)q;
        }
        sigma *= sum;
        if (e & 1) {
            s *= (seq_t)p;
            a3s *= (seq_t)q;
        }
    }
    if (m > 1) {
        uint64_t q = next_prime(m);
        sigma *= (seq_t)(1 + (seq_t)m);
        s *= (seq_t)m;
        a3n *= (seq_t)q;
        a3s *= (seq_t)q;
    }
    if (s == (seq_t)n) return 0;
    return (sigma - s - (seq_t)n) * (a3n - a3s);
}
