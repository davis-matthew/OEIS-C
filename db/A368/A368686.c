/*
 * A368686  a(n) = Product_{j=0..n, k=0..n} (j + k + n).
 * https://oeis.org/A368686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{j=0..n} Product_{k=0..n} (j + k + n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t prod = (seq_t)1;
    idx_t i;

    /* first part: i from n to 2*n, exponent = i - n + 1 */
    for (i = n; i <= 2 * n; ++i) {
        idx_t exp = i - n + 1;          /* 1 .. n+1 */
        prod = seq_mul(prod, pow_seq((seq_t)i, (int)exp));
    }

    /* second part: i from 2*n+1 to 3*n, exponent = 3*n - i + 1 */
    for (i = 2 * n + 1; i <= 3 * n; ++i) {
        idx_t exp = 3 * n - i + 1;      /* 1 .. n */
        prod = seq_mul(prod, pow_seq((seq_t)i, (int)exp));
    }

    return prod;
}
