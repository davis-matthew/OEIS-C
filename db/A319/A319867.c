/*
 * A319867  a(n) = 3*2*1 + 6*5*4 + 9*8*7 + 12*11*10 + ... + (up to the n-th
 *          term).
 * https://oeis.org/A319867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..⌊n/3⌋} (3k)*(3k-1)*(3k-2) + (n%3 == 1 ? (n+2) :
 *            (n%3 == 2 ? (n+1)*n : 0)) (Derivation from the OEIS entry for
 *            k=3)
 * Range:     u8 n <= 8, u16 n <= 29, u32 n <= 476, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* only idx_t to seq_t conversions are needed for strictly integer types */
    seq_t sum = 0;
    idx_t maxk = n / 3;               /* floor(n/3) */
    for (idx_t k = 1; k <= maxk; ++k)
    {
        seq_t j = ((seq_t)3) * ((seq_t)k);       /* j = 3k   */
        sum += j * (j - 1) * (j - 2);            /* add j*(j-1)*(j-2) */
    }

    if (n % 3 == 1)
        sum += ((seq_t)n) + 2;
    else if (n % 3 == 2)
        sum += ((seq_t)n + 1) * ((seq_t)n);

    return sum;
}
