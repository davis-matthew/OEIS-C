/*
 * A251584  a(n) = 4^(n-2) * (n+1)^(n-4) * (3*n^2 + 13*n + 16).
 * https://oeis.org/A251584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(n-2) * (n+1)^(n-4) * (3*n^2 + 13*n + 16)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    /* polynomial part */
    seq_t poly = 3 * n_seq * n_seq + 13 * n_seq + 16;

    /* powers with possible negative exponents handled by division */
    seq_t pow4_exp  = (n >= 2) ? (n - 2)     : (2 - n);
    seq_t pow4      = big_pow(4, (seq_t)pow4_exp);

    seq_t pown1_exp = (n >= 4) ? (n - 4)     : (4 - n);
    seq_t pown1     = big_pow(n_seq + 1, (seq_t)pown1_exp);

    seq_t result = poly;
    if (n >= 2)
        result *= pow4;
    else
        result /= pow4;   /* exact division */

    if (n >= 4)
        result *= pown1;
    else
        result /= pown1;   /* exact division */

    return result;
}
