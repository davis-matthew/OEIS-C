/*
 * A332156  a(n) = 5*(10^(2*n+1)-1)/9 + 10^n.
 * https://oeis.org/A332156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*(10^(2*n+1)-1)/9 + 10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

/* Computes OEIS sequence A332156.
 * a(n) = 5*(10^(2*n+1)-1)/9 + 10^n
 * for all n >= 0.
 */
seq_t a(idx_t n)
{
    idx_t exp = 2 * n + 1;
    seq_t ten_exp = pow10(exp);  /* 10^(2*n+1) */
    seq_t ten_n   = pow10(n);    /* 10^n   */

    /* (10^(2*n+1)-1)/9 is always integer */
    seq_t q = (ten_exp - (seq_t)1) / (seq_t)9;

    /* Multiply by 5 then add 10^n.
     * The division before multiplication prevents overflow
     * for all n whose final result fits seq_t.
     */
    return q * (seq_t)5 + ten_n;
}
