/*
 * A376290  a(n) = Sum_{k=1..n-1} sigma_2(k) * sigma_3(n-k).
 * https://oeis.org/A376290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} sigma_2(k) * sigma_3(n-k)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the supplied sigma_k helper:
   seq_t sigma_k(idx_t m, uint8_t k);
   to obtain the divisor–sum functions. */
#include "../utils/sigma_k.h"

/* Compute the sequence value for any n >= 1. */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 0 */
    if (n <= 1)
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k < n; ++k) {
        /* sigma_2(k) * sigma_3(n-k) */
        seq_t term2 = sigma_k(k,      (uint8_t)2);
        seq_t term3 = sigma_k(n - k,  (uint8_t)3);
        sum += term2 * term3;
    }
    return sum;
}
