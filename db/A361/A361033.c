/*
 * A361033  a(n) = 3*(4*n)!/(n!*(n+1)!^3).
 * https://oeis.org/A361033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3; a(n)=a(n-1)*4*(4*n-1)*(4*n-2)*(4*n-3)/(n+1)^3 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)3;
    }

    seq_t result = (seq_t)3;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t k          = (seq_t)i;                     /* current index as seq_t   */
        seq_t four_k     = (seq_t)4 * k;
        seq_t num_part1  = four_k - (seq_t)1;            /* 4*i - 1                  */
        seq_t num_part2  = four_k - (seq_t)2;            /* 4*i - 2                  */
        seq_t num_part3  = four_k - (seq_t)3;            /* 4*i - 3                  */

        /* numerator: 4*(4*i-1)*(4*i-2)*(4*i-3) */
        seq_t num = (seq_t)4 * num_part1 * num_part2 * num_part3;

        /* denominator: (i+1)^3 */
        seq_t den = k + (seq_t)1;
        den = den * den * den;

        /* compute result = result * num / den
           Use 128‑bit intermediate to avoid overflow on 64‑bit builds   */
        unsigned __int128 prod = (unsigned __int128)result * (unsigned __int128)num;
        result = (seq_t)(prod / den);
    }

    return result;
}
