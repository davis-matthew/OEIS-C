/*
 * A341518  Numbers k such that the primorial base representation of their
 *          arithmetic derivative does not contain digits larger than 1.
 * https://oeis.org/A341518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer (offset 1) whose arithmetic derivative has a
 *            primorial‑base representation comprising only 0s and 1s
 * Range:     u8 n <= 86, u16 n <= 3825, u32 n <= 9146, u64 n <= 9146,
 *            u128 n <= 9146, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

/* Return true iff k satisfies the primorial‑base digit condition. */
static bool is_valid_uint64(uint64_t k) {
    /* For k = 0 or 1 the derivative is 0 → always valid. */
    if (k <= 1) return true;

    /* Arithmetic derivative: k' = Σ e_i * (k / p_i). */
    seq_t d = 0;
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(k, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)(k / primes[i]) * (seq_t)exps[i];
        d += term;
    }

    /* Examine the primorial‑base digits of d. */
    uint64_t p = 2;          /* current prime base */
    seq_t m = d;
    while (m != 0) {
        seq_t digit = m % (seq_t)p;
        if (digit > (seq_t)1) return false;
        m /= (seq_t)p;
        p = next_prime(p);   /* next prime for higher digit */
    }
    return true;
}

/* External linkage required by the build system. */
seq_t a(idx_t n) {
    if (n == 0) return 0;      /* Undefined for n = 0, but harmless */

    idx_t count = 0;
    idx_t k = 0;

    while (true) {
        if (is_valid_uint64((uint64_t)k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
}
