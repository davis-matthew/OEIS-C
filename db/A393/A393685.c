/*
 * A393685  Records in A393684.
 * https://oeis.org/A393685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 12*9^n - 16*3^n + 4*(n-1) + 11
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   mul_small, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Handle offset: a(1) is the first term */
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    /* Compute 9^n and 3^n using the provided big‑integer power routine */
    seq_t pow9 = seq_pow((seq_t)9, n);
    seq_t pow3 = seq_pow((seq_t)3, n);

    /* term = 12*9^n - 16*3^n */
    seq_t term = mul_small(pow9, 12);
    term -= mul_small(pow3, 16);

    /* Add the small linear part 4*(n-1)+11 */
    seq_t linear = (seq_t)(4 * (n - 1) + 11);
    term += linear;

    return term;
}
