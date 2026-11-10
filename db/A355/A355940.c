/*
 * A355940  a(n) = 1 if A003973(n) is a multiple of A000203(n), otherwise 0.
 * https://oeis.org/A355940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sigma(A003961(n)) == 0 (mod sigma(n)), else 0;
 *            A003961 maps each prime to the next prime
 * Range:     u8 n <= 1415, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9006 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* First pass: compute sigma(n) by factoring. */
    seq_t sig = 1;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            sig *= term;
        }
    }
    if (m > 1) sig *= ((seq_t)m + 1);

    /* Second pass: sigma(A003961(n)) modulo sigma(n). */
    seq_t acc = 1 % sig;
    m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            seq_t q = (seq_t)next_prime(p);
            seq_t sum = 1 % sig, pw = 1 % sig;
            for (uint64_t j = 0; j < e; j++) {
                pw = (pw * q) % sig;
                sum = (sum + pw) % sig;
            }
            acc = (acc * sum) % sig;
        }
    }
    if (m > 1) {
        seq_t q = (seq_t)next_prime(m);
        seq_t sum = (1 + q) % sig;
        acc = (acc * sum) % sig;
    }
    return (acc == 0) ? 1 : 0;
}
