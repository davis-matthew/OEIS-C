/*
 * A243994  Consider a decimal number of k>=2 digits x = d_(k)*10^(k-1) +
 *          d_(k-1)*10^(k-2) + … + d_(2)*10 + d_(1) and the transform T(x)->
 *          (d_(k)+d_(k-1) mod 10)*10^(k-1) + (d_(k-1)+d_(k-2) mod
 *          10)*10^(k-2) + … + (d_(2)+d_(1) mod 10)*10 + (d_(1)+d(k) mod 10).
 *          Sequence lists the numbers x such that T(x)=0.
 * https://oeis.org/A243994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is formed by alternating digit d and 10-d for length L =
 *            ⌊(n-1)/9⌋ + 2, where d = ((n-1) mod 9) + 1
 * Range:     u8 n <= 10, u16 n <= 33, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the function has external linkage and uses only integer
   arithmetic as required. */
seq_t a(idx_t n)
{
    idx_t m = n - 1;                         /* zero‑based index   */
    idx_t L = m / 9 + 2;                     /* length of the number */
    int d1 = (int)(m % 9) + 1;               /* first digit (1‑9)   */
    int d2 = 10 - d1;                        /* paired digit       */

    seq_t result = 0;
    for (idx_t i = 0; i < L; ++i)
    {
        int digit = (i & 1) == 0 ? d1 : d2;  /* alternate digits   */
        result = result * 10 + (seq_t)digit;
    }
    return result;
}
