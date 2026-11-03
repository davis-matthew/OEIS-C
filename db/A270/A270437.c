/*
 * A270437  Multiplicative with a(p^e) = p^(e XOR 2e), where XOR is
 *          bitwise-xor.
 * https://oeis.org/A270437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏ p^e‖n p^(e XOR 2e) (multiplicative)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 12167,
 *            u128 n <= 12167, big any n
 * Validated: standard, pass, 66 terms through n = 9621 (2026-11-01)
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
    /* OEIS offset: a(1) = 1. */
    if (n <= 1) return 1;

    /* Factor n into prime powers. */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];
        uint64_t exp = e ^ (e << 1);       /* e XOR 2e */
        seq_t term = ipow(p, (seq_t)exp);
        result *= term;
    }
    return result;
}
