/*
 * A341612  Characteristic function of A341614: a(n) = 1 if sigma(n) <= 2n <
 *          A003961(n), 0 otherwise.
 * https://oeis.org/A341612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sigma(n) <= 2n < A003961(n), else 0 (computed from
 *            the factorization of n)
 * Range:     u8 n <= 62, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8594 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A341612_isp(uint64_t x) {
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A341612_nextp(uint64_t p) {
    uint64_t q = p + 1;
    while (!A341612_isp(q)) q++;
    return q;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    uint64_t m = (uint64_t)n;
    seq_t sig = 1, shift = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = A341612_nextp(p);
            seq_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
                shift *= (seq_t)q;
            }
            sig *= term;
        }
    }
    if (m > 1) {
        sig *= ((seq_t)m + 1);
        shift *= (seq_t)A341612_nextp(m);
    }
    seq_t twice = (seq_t)n * 2;
    return (sig <= twice && twice < shift) ? 1 : 0;
}
