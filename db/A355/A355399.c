/*
 * A355399  a(n) is the failed skew zero forcing number of C^2_n.
 * https://oeis.org/A355399
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n/3) + 2*(ceil(n/(3*floor(n/3) + 1)) -
 *            floor(n/(3*floor(n/3) +1 )) - 1) for n ≥ 11. For n = 3…10 the
 *            values are: 0,1,2,4,3,4,6,5,6,8
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the low‑n values explicitly (offset is 3). */
    if (n < 11) {
        switch (n) {
            case 3:  return 0;
            case 4:  return 1;
            case 5:  return 2;
            case 6:  return 4;
            case 7:  return 3;
            case 8:  return 4;
            case 9:  return 6;
            case 10: return 5;
            default: return 0;          /* n < 3 – behaviour undefined by OEIS, safe return */
        }
    }

    /* For n ≥ 11 we use the closed formula. */
    idx_t k     = n / 3;            /* floor(n/3)  */
    idx_t denom = 3 * k + 1;        /* 3*floor(n/3) + 1  */

    /* If n is a multiple of denom we subtract 2; otherwise return 2*k. */
    if (n % denom == 0)
        return (seq_t)(2 * k) - 2;
    else
        return (seq_t)(2 * k);
}
