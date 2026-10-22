/*
 * A340344  a(n) = ((((((((1723*n mod 1657) mod 861) mod 753) mod 95) * 1602)
 *          + 505) mod 786) mod 506) mod 488.
 * https://oeis.org/A340344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = ((((((1723*n mod 1657) mod 861) mod 753) mod 95) * 1602)
 *            + 505) mod 786) mod 506) mod 488
 * Range:     u8 n <= 3, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the sequence is defined only for n >= 1; the call for n=0 is undefined */
    idx_t t = (1723 * n) % 1657;   /* 1723*n mod 1657 */
    t = t % 861;                   /* result mod 861 */
    t = t % 753;                   /* result mod 753 */
    t = t % 95;                    /* result mod 95 */
    t = (t * 1602 + 505) % 786;    /* (result*1602+505) mod 786 */
    t = t % 506;                   /* result mod 506 */
    t = t % 488;                   /* final modulus */
    return (seq_t)t;
}
