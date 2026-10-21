/*
 * A273669  Decimal representation ends with either 2 or 9.
 * https://oeis.org/A273669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A273669: decimal numbers ending with 2 or 9, in increasing order.
   Formula: a(n) = 10 * floor((n-1)/2) + (n mod 2 == 1 ? 2 : 9),  n ≥ 1
*/
seq_t a(idx_t n)
{
    if (n < 1U)
        return 0;               /* undefined for n < 1, return 0 as a safe default */

    seq_t half = (seq_t)(n - 1) / 2;     /* floor((n-1)/2) */
    seq_t result = half * 10;            /* 10 * floor((n-1)/2) */

    if (n & 1U)                          /* n is odd */
        result += 2;
    else                                 /* n is even */
        result += 9;

    return result;
}
