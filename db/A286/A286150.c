/*
 * A286150  Square array read by antidiagonals: A(n,k) = T(n XOR k, min(n,k)),
 *          where T(n,k) is sequence A001477 considered as a two-dimensional
 *          table, and XOR is bitwise-xor (A003987).
 * https://oeis.org/A286150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(n,k) read by antidiagonals, where A(n,k) = T(n XOR k,
 *            min(n,k)) * and T(a,b) = ((a+b)*(a+b+1))/2 + a (Cantor pairing
 *            with first argument a). * The reading order is (0,0), (0,1),
 *            (1,0), (0,2), (1,1), (2,0), ... * Function returns the nth term,
 *            n >= 0
 * Range:     u8 n <= 126, u16 n <= 9400, u32 n <= 9400, u64 n <= 9400,
 *            u128 n <= 9400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the antidiagonal index d such that n lies in it.
     * Count holds the number of entries before diagonal d. */
    idx_t d = 0;
    idx_t count = 0;
    while (count + d + 1 <= n) {
        count += d + 1;  /* entries added by diagonal d */
        d++;
    }

    /* Position (i,k) within the array */
    idx_t offset = n - count;   /* offset along diagonal d */
    idx_t i = offset;           /* row coordinate */
    idx_t k = d - offset;       /* column coordinate */

    /* Apply the definition A(n,k) = T(n XOR k, min(n,k)) */
    idx_t a_val = i ^ k;                 /* first argument of T */
    idx_t b = (i < k) ? i : k;           /* second argument of T */

    idx_t sum = a_val + b;               /* a_val + b used in the pairing */
    seq_t triangular = (seq_t)sum * (seq_t)(sum + 1) / 2;
    return triangular + (seq_t)a_val;    /* final value of T(a_val,b) */
}
