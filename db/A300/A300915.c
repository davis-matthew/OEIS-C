/*
 * A300915  Order of the group PSL(2,Z_n).
 * https://oeis.org/A300915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = |PSL(2, Z_n)| = A000056(n)/A060594(n) Multiplicative:
 *            a(2) = 6, a(4) = 24, a(2^e) = 3*2^(3e-4) for e > 2, a(p^e) =
 *            (p^2-1)*p^(3e-2)/2 for odd primes p
 * Range:     u8 n <= 8, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* Factor n into primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t e = (idx_t)exps[i];
        seq_t term;

        if (p == 2ULL) {
            if (e == 1)
                term = 6;
            else if (e == 2)
                term = 24;
            else
                term = 3 * pow_seq((seq_t)2, 3 * e - 4);
        } else {
            seq_t pp   = (seq_t)p * (seq_t)p;
            seq_t part1 = (pp - 1) / 2;          /* (p^2-1)/2  */
            seq_t part2 = pow_seq((seq_t)p, 3 * e - 2);
            term = part1 * part2;
        }

        result *= term;
    }

    return result;
}
