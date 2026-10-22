/*
 * A371560  a(n) = n!*(n+1)!^3.
 * https://oeis.org/A371560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*((n+1)!)^3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   factorial, pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow_seq.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t f_n   = factorial(n);
    seq_t f_np1 = factorial(n + 1);
    seq_t cube  = pow_seq(f_np1, 3);
    return seq_mul(f_n, cube);
}
