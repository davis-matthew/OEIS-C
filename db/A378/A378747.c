/*
 * A378747  a(n) = A048673(n) - A001065(n).
 * https://oeis.org/A378747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + (A003961(n)+1)/2 - A000203(n)
 * Range:     u8 n <= 47, u16 n <= 2047, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 98 terms through n = 9756 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    seq_t sig = 1;
    seq_t shifted = 1;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
                shifted *= (seq_t)q;
            }
            sig *= term;
        }
    }
    if (m > 1) {
        sig *= ((seq_t)m + 1);
        shifted *= (seq_t)next_prime(m);
    }
    seq_t half = (shifted + (seq_t)1) / (seq_t)2;
    return (seq_t)n + half - sig;
}
