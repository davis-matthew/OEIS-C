/*
 * A351545  a(n) is the largest unitary divisor of sigma(n) such that its
 *          every prime factor p also divides A003961(n), and
 *          valuation(sigma(n),p) >= valuation(A003961(n),p).
 * https://oeis.org/A351545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{p^e || sigma(n)} p^(e*[p | A003961(n) and
 *            p^(1+e) does not divide A003961(n)])
 * Range:     u8 n <= 102, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 118 terms through n = 8911 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static seq_t A351545_sigma(uint64_t n)
{
    seq_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) result *= ((seq_t)m + 1);
    return result;
}

seq_t a(idx_t n)
{
    seq_t s = A351545_sigma((uint64_t)n);
    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; m > 1; p += (p == 2 ? 1 : 2)) {
        uint64_t pr = 0;
        int k = 0;
        if (p * p > m) {
            pr = m;
            k = 1;
            m = 1;
        } else if (m % p == 0) {
            pr = p;
            while (m % p == 0) { m /= p; k++; }
        } else {
            continue;
        }
        uint64_t q = next_prime(pr);
        int e = 0;
        seq_t qe = 1;
        seq_t t = s;
        while (t % (seq_t)q == 0) {
            t /= (seq_t)q;
            e++;
            qe *= (seq_t)q;
        }
        if (k >= 1 && k <= e) result *= qe;
    }
    return result;
}
