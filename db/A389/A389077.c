/*
 * A389077  Sum of all other proper divisors of primeshifted n except its
 *          squarefree part: a(n) = A001065(A003961(n)) - A007913(A003961(n)).
 * https://oeis.org/A389077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(A003961(n)) - A003961(n) - A007913(A003961(n));
 *            factor n, shift each prime p to next_prime(p), keep exponents
 * Range:     i8 n <= 35, i16 n <= 1151, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9770 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    seq_t sigma_m = 1;
    seq_t sqfree = 1;
    seq_t m_val = 1;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p != 0) continue;
        uint64_t q = next_prime(p);
        int e = 0;
        while (m % p == 0) {
            m /= p;
            e++;
        }
        seq_t term = 1, pw = 1;
        for (int i = 0; i < e; i++) {
            pw *= (seq_t)q;
            term += pw;
        }
        sigma_m *= term;
        m_val *= pw;
        if (e & 1) sqfree *= (seq_t)q;
    }
    if (m > 1) {
        uint64_t q = next_prime(m);
        sigma_m *= ((seq_t)1 + (seq_t)q);
        m_val *= (seq_t)q;
        sqfree *= (seq_t)q;
    }
    return sigma_m - m_val - sqfree;
}
