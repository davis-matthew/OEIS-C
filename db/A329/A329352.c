/*
 * A329352  a(n) = Product_{d|n} A019565(d)^A010051(n/d).
 * https://oeis.org/A329352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{d|n} A019565(d)^A010051(n/d) (equivalent to Π_{p|n}
 *            A019565(n/p), p distinct primes dividing n)
 * Range:     u8 n <= 27, u16 n <= 29, u32 n <= 209, u64 n <= 1019,
 *            u128 n <= 8192, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* first 64 primes – used for A019565 */
static const unsigned int PRIMES[64] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
    41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131, 137, 139,
    149, 151, 157, 163, 167, 173, 179, 181, 191, 193,
    197, 199, 211, 223, 227, 229, 233, 239, 241, 251,
    257, 263, 269, 271, 277, 281, 283, 293, 307, 311
};

/* Compute A019565(n): product of primes whose 1‑based indices are the set
   bits of the binary representation of n.  Bits are processed 0‑based
   (bit 0 -> first prime, etc.). */
static seq_t
A019565(idx_t n)
{
    seq_t val = 1;
    unsigned int bit = 0;
    while (n) {
        if (n & 1U) {
            val *= (seq_t)PRIMES[bit];
        }
        n >>= 1;
        ++bit;
    }
    return val;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;   /* undefined but harmless */

    /* factorize n – primes[] holds distinct primes, exps[] their exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t d = n / primes[i];      /* n/p for each distinct prime p */
        seq_t term = A019565(d);
        result *= term;
    }
    return result;
}
