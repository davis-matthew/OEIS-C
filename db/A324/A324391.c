/*
 * A324391  Fully multiplicative with a(p^e) = A070939(p)^e, where A070939(p)
 *          gives the length of the binary representation of p.
 * https://oeis.org/A324391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (⌊log₂(p)⌋ + 1)^e (i.e. a(n) is the product
 *            over prime powers of the binary length of the prime, raised to
 *            the prime's exponent)
 * Range:     u8 n <= 255, u16 n <= 65535, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 110 terms through n = 8926 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned bitlen_uint64(idx_t x) {
    unsigned len = 0;
    while (x) { ++len; x >>= 1; }
    return len;
}

static seq_t int_pow(seq_t base, idx_t exp) {
    seq_t r = 1;
    while (exp) { r *= base; --exp; }
    return r;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    seq_t result = 1;
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            seq_t base = (seq_t)bitlen_uint64(p);
            result *= int_pow(base, exp);
        }
    }

    if (tmp > 1) {
        seq_t base = (seq_t)bitlen_uint64(tmp);
        result *= base; /* exponent is 1 */
    }

    return result;
}
