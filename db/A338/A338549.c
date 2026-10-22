/*
 * A338549  a(n) = n^4 * Sum_{d|n} (-1)^(n/d + 1) * mu(d) / d^4.
 * https://oeis.org/A338549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = n^4 * Sum_{d|n} (-1)^(n/d + 1) * mu(d) / d^4 Sequence is
 *            multiplicative with a(1) = 1 a(2) = -17 a(2^e) = -15 *
 *            2^(4*(e-1)) for e > 1 a(p^e) = (p^4-1) * p^(4*(e-1)) for odd
 *            prime p The function below implements this using simple trial
 *            division factorisation and per-prime multiplicative con...
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 213, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   pow_uint_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_uint_local.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t nn = n;
    seq_t result = (seq_t)1;

    /* trial‑division factorisation */
    while (nn > 1) {
        idx_t p = 0;
        for (p = 2; p * p <= nn; ++p) {
            if (nn % p == 0)
                break;
        }
        if (p * p > nn)          /* remaining nn is prime */
            p = nn;

        unsigned int exp = 0;
        while (nn % p == 0) {
            nn /= p;
            ++exp;
        }

        seq_t term;
        if (p == 2) {
            if (exp == 1) {
                term = (seq_t)-17;
            } else {
                seq_t powPow = pow_uint_local(2, 4 * (exp - 1));
                term = (seq_t)-15 * powPow;
            }
        } else {
            seq_t powp4 = pow_uint_local(p, 4);
            seq_t powpe = pow_uint_local(p, 4 * (exp - 1));
            term = (powp4 - 1) * powpe;
        }

        result *= term;
    }

    return result;
}
