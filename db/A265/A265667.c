/*
 * A265667  Permutation of nonnegative integers: a(n) = n + floor(n/3)*(-1)^(n
 *          mod 3).
 * https://oeis.org/A265667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n/3)*(-1)^(n mod 3) (permuting nonnegative
 *            integers)
 * Range:     u8 n <= 191, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = floor(n / 3) */
    idx_t k = n / 3;
    idx_t r = n % 3;            /* n mod 3 */

    if (r == 0) {
        return (seq_t)4 * k;        /* a(3k) = 4k   */
    } else if (r == 1) {
        return (seq_t)(2 * k + 1);  /* a(3k+1) = 2k + 1 */
    } else {                      /* r == 2 */
        return (seq_t)(4 * k + 2);  /* a(3k+2) = 4k + 2 */
    }
}
