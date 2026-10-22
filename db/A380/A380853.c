/*
 * A380853  Number of ways to place six distinct positive integers on a
 *          triangle, three on the corners and three on the sides such that
 *          the sum of the three values on each side is n.
 * https://oeis.org/A380853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n³-80*n²+388*n-520)/120 + (n mod 2)*(20-3*n)/8 +
 *            ((2*n²+1) mod 3)*(2*n-14)/3 + ((3*n) mod 5)/5 - ((n²+3*n+2) mod
 *            4)/2
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 64‑bit arithmetic for safety; intermediate values fit easily
       for n up to several thousand (the required limit). */
    int64_t nn   = (int64_t)n;
    int64_t t1   = 5*nn*nn*nn - 80*nn*nn + 388*nn - 520;            /* /120   */
    int64_t t2   = 15 * ((nn & 1)) * (20 - 3*nn);                    /* /8     */
    int64_t t3   = 40 * ((2*nn*nn + 1) % 3) * (2*nn - 14);           /* /3     */
    int64_t t4   = 24 * (3*nn % 5);                                 /* /5     */
    int64_t t5   = 60 * ((nn*nn + 3*nn + 2) % 4);                    /* /2     */

    int64_t num  = t1 + t2 + t3 + t4 - t5;                           /* numerator for /120 */

    return (seq_t)(num / 120);
}
