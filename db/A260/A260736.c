/*
 * A260736  a(0) = 0; for n >= 1, a(n) = A000035(n) + a(A257684(n)); in the
 *          factorial representation of n the number of digits with maximal
 *          possible value allowed in its location.
 * https://oeis.org/A260736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of factorial base digits that equal their maximal
 *            allowed value. Computed by converting n to factorial
 *            representation and counting positions where digit == position.
 *            a(0) = 0
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;          /* number of max‑allowed digits found */
    idx_t m = n;              /* working copy of the index */

    for (idx_t j = 1; m > 0; ++j) {
        idx_t digit = m % (j + 1);   /* digit for position j (1‑based) */
        if (digit == j)
            ++count;                /* it attains its maximum allowed value */
        m /= (j + 1);                /* move to next factorial position */
    }

    return count;
}
