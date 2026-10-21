/*
 * A245478  Numbers k such that the k-th cyclotomic polynomial has a root mod
 *          5.
 * https://oeis.org/A245478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-3), a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 83, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three special cases directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* Compute the residue modulo 3 and the corresponding base factor */
    idx_t r   = n % 3;
    idx_t exp;          /* exponent of 5 */
    seq_t base;         /* 1, 2 or 4 */

    if (r == 1) {           /* a(3j+1) = 2^0 * 5^j */
        exp  = (n - 1) / 3;
        base = (seq_t)1;
    } else if (r == 2) {    /* a(3j+2) = 2^1 * 5^j */
        exp  = (n - 2) / 3;
        base = (seq_t)2;
    } else {                /* r == 0 : a(3j+3) = 2^2 * 5^j */
        exp  = (n - 3) / 3;
        base = (seq_t)4;
    }

    /* Compute 5^exp iteratively (exp <= n/3 <= 3333 for n <= 10000) */
    seq_t pow5 = 1;
    for (idx_t i = 0; i < exp; ++i)
        pow5 *= (seq_t)5;

    return base * pow5;
}
