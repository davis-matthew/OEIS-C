/*
 * A284797  Write in base k, complement, reverse. Case k = 3.
 * https://oeis.org/A284797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = write n in base 3, replace each digit d by 2-d (the
 *            complement in base 3), reverse the digit order, and interpret
 *            the result as a base‑3 number
 * Range:     u8 n <= 242, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* when n is 0 the base‑3 representation is just "0"
       → complement "2", reversing does not change it */
    if (n == 0)
        return 2;

    seq_t res = 0;
    /* read n digit by digit in base 3 (least significant first),
       compute the complemented digit and build the reversed number
       by prepending digits (i.e. multiplying by 3 before adding). */
    while (n > 0)
    {
        idx_t d = n % 3;          /* current base‑3 digit (0,1,2)   */
        res = res * 3 + (2 - d);  /* complement and prepend digit  */
        n /= 3;
    }
    return res;
}
