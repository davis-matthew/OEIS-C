/*
 * A341613  Characteristic function for A341615: a(n) = 1 if sigma(n) < 2n <
 *          A003961(n), otherwise 0.
 * https://oeis.org/A341613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sigma(n) < 2n < A003961(n), else 0
 *            (A294934(n)*A252742(n)); computed by trial-division
 *            factorization
 * Range:     u8 n <= 62, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8396 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A341613_next_prime(uint64_t p) {
    uint64_t q = p + 1;
    for (;; q++) {
        bool ok = (q >= 2);
        for (uint64_t d = 2; d * d <= q; d++) {
            if (q % d == 0) { ok = false; break; }
        }
        if (ok) return q;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    uint64_t m = (uint64_t)n;
    seq_t sig = 1;
    seq_t shifted = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = A341613_next_prime(p);
            seq_t term = 1, pw = 1;
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
        shifted *= (seq_t)A341613_next_prime(m);
    }
    seq_t two_n = (seq_t)n * 2;
    return (sig < two_n && two_n < shifted) ? 1 : 0;
}
