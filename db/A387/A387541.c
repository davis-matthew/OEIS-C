/*
 * A387541  Sum of the even parts in the partitions of n into 2 parts.
 * https://oeis.org/A387541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (2*n^2-2*n-1-(2*n-1)*(-1)^n+n*(1+(-1)^(n/2))*(1+(-1)^n))/8
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Simplified version:
       If n is even:
           If n%4==0: a = n^2 / 4
           Else:     a = n*(n-2)/4
       Else (n odd): a = (n-1)*(n+1)/4
    */
    if ((n & 1ULL) == 0) {                /* n even */
        if ((n & 2ULL) == 0) {            /* n % 4 == 0 */
            return (seq_t)n * (seq_t)n / ((seq_t)4);
        } else {                          /* n % 4 == 2 */
            return (seq_t)n * (seq_t)(n - 2) / ((seq_t)4);
        }
    } else {                              /* n odd */
        return ((seq_t)(n - 1) * (seq_t)(n + 1)) / ((seq_t)4);
    }
}
