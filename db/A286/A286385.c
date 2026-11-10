/*
 * A286385  a(n) = A003961(n) - A000203(n).
 * https://oeis.org/A286385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003961(n) - A000203(n), where A003961 is completely
 *            multiplicative with prime(k) -> prime(k+1), and A000203 =
 *            sigma(n)
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 112 terms through n = 9998 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    seq_t shifted = 1;
    seq_t sig = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;

    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t q = (seq_t)next_prime(p);
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
                shifted *= q;
            }
            sig *= term;
        }
    }
    if (m > 1) {
        shifted *= (seq_t)next_prime(m);
        sig *= ((seq_t)m + 1);
    }
    return shifted - sig;
}
