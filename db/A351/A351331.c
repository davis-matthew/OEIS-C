/*
 * A351331  a(n) = (n+1)*(n^n) - n + 1.
 * https://oeis.org/A351331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n^n) - n + 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n=0 – return 0 */

    seq_t N = (seq_t)n;                 /* cast index to seq_t */
    seq_t pow_n = big_pow(N, N);        /* N^N */
    seq_t result = (N + 1) * pow_n - N + 1;
    return result;
}
