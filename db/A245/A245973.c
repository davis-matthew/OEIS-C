/*
 * A245973  Tower of 6s mod n.
 * https://oeis.org/A245973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Tower of 6's modulo n (6^(6^(6^...)) mod n)
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/calc_phi.h"
#include "../utils/mod_pow.h"

static seq_t tower_mod(seq_t m)
{
    if (m == (seq_t)1) {
        return (seq_t)0;
    }
    seq_t phi = calc_phi(m);
    seq_t exp = tower_mod(phi);
    if (gcd((seq_t)6, m) != (seq_t)1) {
        exp += phi;
    }
    return mod_pow((seq_t)6, (uint64_t)exp, m);
}

seq_t a(idx_t n)
{
    return tower_mod((seq_t)n);
}
