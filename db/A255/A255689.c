/*
 * A255689  Convert n to base 4, move the most significant digit to the least
 *          significant one and convert back to base 10.
 * https://oeis.org/A255689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 319, u16 n <= 9695, u32 n <= 9695, u64 n <= 9695,
 *            u128 n <= 9695, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 0
 *          a(n) = ((n % 4^(k-1)) * 4) + (n / 4^(k-1)),
 *          where k = floor(log_4(n)) + 1
 *          (i.e., rotate the most significant base‑4 digit to the least
 *          significant digit and interpret the result in base‑10).
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* find the number of base‑4 digits of n (k) */
    idx_t tmp = n;
    idx_t k = 0;
    while (tmp != 0) {
        k++;
        tmp /= 4;
    }

    /* power = 4^(k-1)  (equals 2^(2*(k-1))) */
    idx_t power = (idx_t)1 << (2 * (k - 1));

    /* most significant digit and remaining part */
    idx_t msd = n / power;
    idx_t rest = n % power;

    /* rotate digit */
    seq_t result = (seq_t)(rest * (seq_t)4 + msd);
    return result;
}
