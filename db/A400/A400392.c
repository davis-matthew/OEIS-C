/*
 * A400392  a(n) = Sum_{d|n} gcud(d, n/d), where gcud is the greatest common
 *          unitary divisor.
 * https://oeis.org/A400392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} gcud(d, n/d) Multiplicative with a(p^e) = e+1
 *            if e is odd, else p^(e/2)+e
 * Range:     u8 n <= 3599, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9035 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* out of range, but safe */
    /* factorization */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = (idx_t)exps[i];
        seq_t term;
        if (e & 1) {                /* e is odd */
            term = (seq_t) e + 1;
        } else {                    /* e is even */
            seq_t pow = ipow((seq_t)primes[i], (seq_t)(e / 2));
            term = pow + (seq_t) e;
        }
        result = result * term;
    }
    return result;
}
