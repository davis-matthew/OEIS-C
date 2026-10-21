/*
 * A245593  a(n) = (R_n)^n.
 * https://oeis.org/A245593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((10^n - 1)/9)^n (repdigit R_n raised to the n‑th power)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used; offset is 1 */
    seq_t rep = pow10(n);          /* 10^n  */
    rep = rep - 1;                 /* 10^n - 1 */
    rep = rep / 9;                 /* (10^n - 1)/9  -> R_n  */
    return big_pow(rep, (seq_t)n); /* R_n^n  */
}
