/*
 * A392225  Concatenations x||1||x for numbers x.
 * https://oeis.org/A392225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 65, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n * 10^(digits(n)+1) + 10^digits(n) + n
 * where digits(n) is the number of decimal digits of n
 */
seq_t a(idx_t n)
{
    /* count the number of decimal digits of n */
    idx_t tmp = n;
    int d = 0;
    do
    {
        tmp /= 10;
        ++d;
    } while (tmp != 0);

    /* compute 10^d and 10^(d+1) using seq_t arithmetic */
    seq_t pow10_d = 1;
    for (int i = 0; i < d; ++i)
        pow10_d *= (seq_t)10;

    seq_t pow10_dplus1 = pow10_d * (seq_t)10;

    /* construct the concatenation n||1||n :
       n * 10^(d+1) + 1 * 10^d + n */
    seq_t result = ((seq_t)n) * pow10_dplus1 + pow10_d + ((seq_t)n);
    return result;
}
