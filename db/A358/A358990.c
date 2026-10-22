/*
 * A358990  a(n) is the product of the first n odd numbers not divisible by 5.
 * https://oeis.org/A358990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} A045572(k) (product of the first n odd numbers
 *            not divisible by 5)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    if (n == 0) {
        return prod;           /* a(0) = 1 */
    }

    idx_t count = 0;
    idx_t candidate = 1;        /* start with the first odd number */

    while (count < n) {
        if (candidate % 5 != 0) {    /* not divisible by 5 */
            prod *= (seq_t)candidate;
            count++;
        }
        candidate += 2;              /* next odd number */
    }
    return prod;
}
