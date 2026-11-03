/*
 * A253719  Least k>0 such that n AND (n^k) <= 1, where AND denotes the
 *          bitwise AND operator.
 * https://oeis.org/A253719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 9919, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 101 terms through n = 9919 (2026-11-01)
 * Helpers:   ilog2, pow2, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A253719.
   For each index n, it finds the minimal k>0 such that
   n AND (n^k) <= 1.  It uses modular exponentiation
   to avoid handling huge numbers. */

/* required utilities */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ilog2.h"
#include "../utils/pow2.h"
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    /* a(0) is 1 because 0^k = 0 and 0 & 0 = 0 <= 1 */
    if (n == 0)
        return (seq_t)1;

    /* number of bits needed to represent n */
    idx_t bitlen = ilog2(n) + 1;
    seq_t mod = pow2(bitlen);          /* modulus 2^bitlen */
    seq_t sn = (seq_t)n;               /* cast index to seq_t */

    for (idx_t k = 1; ; ++k)
    {
        /* compute n^k modulo 2^bitlen */
        seq_t pow_mod = powmod(sn, (uint64_t)k, mod);
        seq_t and_val = sn & pow_mod;  /* bitwise AND with original n */

        /* stop when the result is 0 or 1 */
        if (and_val <= (seq_t)1)
            return (seq_t)k;
    }
}
