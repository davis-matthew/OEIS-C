/*
 * A341090  Fully multiplicative: for any prime p, if the reversal of p in
 *          base 10, say q, is prime, then a(p) = q, otherwise a(p) = p.
 * https://oeis.org/A341090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, mult
 * Formula:   a(n) = multiplicative function defined on primes p: a(p) =
 *            rev(p) if rev(p) is prime, else a(p) = p. For general n, a(n) =
 *            Π a(p)^e where n = Π p^e
 * Range:     u8 n <= 67, u16 n <= 2532, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/rev.h"            /* rev(idx_t) */
#include "../utils/is_prime.h"       /* is_prime(idx_t) */

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for n=0, but safe */
    if (n == 1) return 1;

    seq_t result = 1;
    uint64_t tmp = (uint64_t)n;

    /* handle factor 2 separately */
    unsigned exp = 0;
    while ((tmp & 1ULL) == 0ULL) {
        tmp >>= 1;
        ++exp;
    }
    if (exp) {
        seq_t p_val = 2;
        if (is_prime(rev(2))) p_val = rev(2);
        seq_t pow = 1;
        for (unsigned i = 0; i < exp; ++i) pow *= p_val;
        result *= pow;
    }

    /* trial division for odd factors */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0ULL) {
            exp = 0;
            while (tmp % p == 0ULL) {
                tmp /= p;
                ++exp;
            }
            seq_t p_val = (seq_t)p;
            if (is_prime(rev(p))) p_val = rev(p);
            seq_t pow = 1;
            for (unsigned i = 0; i < exp; ++i) pow *= p_val;
            result *= pow;
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1ULL) {
        seq_t p_val = (seq_t)tmp;
        if (is_prime(rev(tmp))) p_val = rev(tmp);
        result *= p_val;
    }

    return result;
}
