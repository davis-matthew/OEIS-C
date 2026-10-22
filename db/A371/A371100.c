/*
 * A371100  Array A read by upward antidiagonals in which the entry A(n,k) in
 *          row n and column k is defined by A(n, k) = 4^n*(6*k - 3 -
 *          2*(-1)^n) + (4^n - 1)/3, n,k >= 1.
 * https://oeis.org/A371100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the array A(n,k) with
 *            A(n,k)=4^n*(6*k-3-2*(-1)^n)+(4^n-1)/3 upward antidiagonally (i
 *            increases along each diagonal)
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 105, u64 n <= 465,
 *            u128 n <= 1953, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"   /* big_pow(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* Determine the diagonal s = i + j (s >= 2) that contains the nth term. */
    idx_t s = 2;
    idx_t count = 1;               /* number of terms up to current diagonal */

    while (count < n) {
        ++s;
        count += s - 1;            /* add size of this diagonal */
    }

    /* Terms before this diagonal. */
    idx_t total_before = count - (s - 1);
    idx_t offset = n - total_before - 1;      /* 0‑based position on the diagonal */

    /* Coordinates on the diagonal read downward (i increasing). */
    idx_t i = s - 1 - offset;
    idx_t j = 1 + offset;

    /* 4^i using the provided big‑integer power routine. */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)i);

    /* Compute the linear coefficient: 6*j-1 for odd i, 6*j-5 for even i. */
    seq_t coeff;
    if (i & 1)                     /* i is odd */
        coeff = (seq_t)(6 * j - 1);
    else                           /* i is even */
        coeff = (seq_t)(6 * j - 5);

    /* Final value. */
    seq_t term1 = pow4 * coeff;
    seq_t term2 = (pow4 - (seq_t)1) / (seq_t)3;
    return term1 + term2;
}
