/*
 * A291271  The arithmetic function v_4(n,2).
 * https://oeis.org/A291271
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - gcd(n,4)) / 2 Equivalent to a(n) = (n - ((n%4==0)?4
 *            : ((n%2==0)?2 : 1))) / 2
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* values below the offset are irrelevant */
    idx_t g;
    /* Compute gcd(n,4) without calling the gcd utility */
    if ((n % 4) == 0)
        g = 4;
    else if ((n % 2) == 0)
        g = 2;
    else
        g = 1;
    return ((seq_t)n - (seq_t)g) / 2;
}
