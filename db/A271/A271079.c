/*
 * A271079  Residues (mod 32) of partial sums of Fibonacci numbers starting
 *          with F(2).
 * https://oeis.org/A271079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(n+4)-2) mod 32
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   fib_pair_mod32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_pair_mod32.h"

seq_t a(idx_t n)
{
    /* Reduce the index modulo the Pisano period 48 for modulus 32. */
    idx_t idx = (n % 48ULL + 4ULL) % 48ULL;

    uint32_t fn, dummy;
    fib_pair_mod32(idx, &fn, &dummy);

    /* Compute (F(n+4)-2) mod 32, i.e., (fn - 2) mod 32. */
    uint32_t res = (fn + 30) & 31;   /* fn - 2 ≡ fn + 30 (mod 32) */

    return (seq_t)res;
}
