/*
 * A342921  a(n) = A003415(A019565(n)).
 * https://oeis.org/A342921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A003415(A019565(n)); A019565(n) = Product of prime(k+1)
 *            over set bits k of n, which is squarefree, so its arithmetic
 *            derivative is Sum_{p|m} m/p. Computed incrementally over the set
 *            bits: (prod, d) -> (prod*p, d*p + prod)
 * Range:     u8 n <= 22, u16 n <= 118, u32 n <= 1022, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 86 terms through n = 7507 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A342921_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t d = 0;
    unsigned bit = 0;
    while (n != 0) {
        if (n & 1) {
            seq_t p = (seq_t)A342921_primes[bit];
            d = d * p + prod;
            prod *= p;
        }
        n >>= 1;
        ++bit;
    }
    return d;
}
