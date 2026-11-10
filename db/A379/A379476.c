/*
 * A379476  a(n) = 1 if A326057(n) > 1, otherwise 0, where A326057(n) =
 *          gcd(A003961(n)-2n, A003961(n)-sigma(n)).
 * https://oeis.org/A379476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if gcd(A003961(n)-2n, A003961(n)-sigma(n)) > 1, else 0
 * Range:     u8 n <= 53, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 136 terms through n = 9227 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static bool a379476_isprime(uint64_t m)
{
    if (m < 2) return false;
    for (uint64_t d = 2; d * d <= m; d++)
        if (m % d == 0) return false;
    return true;
}

static uint64_t a379476_nextprime(uint64_t p)
{
    uint64_t q = p + 1;
    while (!a379476_isprime(q)) q++;
    return q;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    seq_t B = 1, sg = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = a379476_nextprime(p);
            seq_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                B *= (seq_t)q;
                pw *= (seq_t)p;
                term += pw;
            }
            sg *= term;
        }
    }
    if (m > 1) {
        B *= (seq_t)a379476_nextprime(m);
        sg *= ((seq_t)m + 1);
    }
    seq_t two_n = (seq_t)n * 2;
    seq_t d1 = (B > two_n) ? (B - two_n) : (two_n - B);
    seq_t d2 = (B > sg) ? (B - sg) : (sg - B);
    seq_t g = gcd(d1, d2);
    return (g > 1) ? (seq_t)1 : (seq_t)0;
}
