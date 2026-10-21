/*
 * A267526  Binary representation of the n-th iteration of the "Rule 141"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n * (100^floor(n/2) - 1) / 99 + (10^(n-1) - 1) / 9 (n
 *            > 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A267526: binary representation of the n‑th iteration of
 * Rule 141 elementary cellular automaton starting with a single ON cell */
#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    /* for n >= 2 */
    idx_t m = n / 2;                 /* floor(n/2) */

    /* 10^n and 10^(2*m) */
    seq_t ten_pow_n     = pow10(n);
    seq_t ten_pow_2m    = pow10(2 * m);

    /* first term: 10^n * (10^(2*m) - 1) / 99 */
    seq_t term1 = ((ten_pow_2m - 1) / 99) * ten_pow_n;

    /* second term: (10^(n-1) - 1) / 9 */
    seq_t term2 = (pow10(n - 1) - 1) / 9;

    return term1 + term2;
}
