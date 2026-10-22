/*
 * A331737  Multiplicative with a(p^e) = p^A000265(e), where A000265(x) gives
 *          the odd part of x.
 * https://oeis.org/A331737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} p^{odd(e)}, where odd(e) is the odd part of the
 *            exponent e
 * Range:     u8 n <= 256, u16 n <= 8993, u32 n <= 8993, u64 n <= 8993,
 *            u128 n <= 8993, big any n
 * Helpers:   seq_pow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow_uint64.h"

/* helper: exponentiation by squaring for seq_t (any unsigned type) */

seq_t a(idx_t n) {
    /* n is guaranteed ≥ 1 by the sequence definition */
    uint64_t tmp = (uint64_t)n;   /* cast to native type for factorisation */
    seq_t res = 1;

    /* trial division up to sqrt(tmp) */
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            /* odd part of exponent e */
            while ((e & 1ULL) == 0ULL) e >>= 1ULL;   /* divide by 2 while even */

            /* multiply by p^odd(e) */
            seq_t pow = seq_pow_uint64((seq_t)p, e);
            res *= pow;
        }
    }

    /* remaining prime factor (if any) */
    if (tmp > 1) {
        /* exponent is 1 → odd part is 1 */
        res *= (seq_t)tmp;
    }

    return res;
}
