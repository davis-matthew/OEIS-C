/*
 * A307151  Row 5 of array in A059897.
 * https://oeis.org/A307151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n if v5(n) is even, n/5 if v5(n) is odd where v5(n) is
 *            the exponent of the largest power of 5 dividing n. This
 *            implements OEIS A307151: a self‑inverse permutation swapping
 *            pairs whose ratio is 1:5
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine parity of the 5‑adic valuation of n. */
    idx_t t = n;
    unsigned parity = 0;            /* 0 = even, 1 = odd */
    while ((t % 5) == 0) {
        parity ^= 1;
        t /= 5;
    }

    if (parity == 0) {              /* even exponent → multiply by 5 */
        return (seq_t)n * 5;
    } else {                        /* odd exponent → divide by 5 */
        return (seq_t)n / 5;
    }
}
