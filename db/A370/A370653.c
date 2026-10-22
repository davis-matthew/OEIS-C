/*
 * A370653  Number of permutations of [n] having exactly three adjacent
 *          4-cycles.
 * https://oeis.org/A370653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 18, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A370653
   Formula: a(n) = (1/6) * Sum_{k=0..⌊n/4⌋-3} (-1)^k * (n-3k-9)! / k!  for n ≥ 12,
            a(n) = 0 for n < 12.
   Implementation avoids overflow by cancelling the factor 6 from the
   factorial product as it is built. */
seq_t a(idx_t n)
{
    /* For indices below the offset, the value is 0. */
    if (n < 12)
        return (seq_t)0;

    idx_t maxK = (n / 4) - 3;            /* floor(n/4) - 3 */
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxK; ++k) {
        idx_t upper = n - 3 * k - 9;     /* upper bound of the product */
        seq_t term = 1;

        /* remaining factors of 6 that are still to be cancelled */
        unsigned int rem2 = 2;
        unsigned int rem3 = 3;

        for (idx_t i = k + 1; i <= upper; ++i) {
            unsigned int val = (unsigned int)i;

            /* cancel one factor of 2 and/or 3 if possible */
            if (rem2 && (val & 1) == 0) {
                val >>= 1;
                rem2 = 0;
            }
            if (rem3 && (val % 3) == 0) {
                val /= 3;
                rem3 = 0;
            }

            term *= (seq_t)val;
        }

        /* By construction the product is divisible by 6; we have removed it
           by cancelling the factors 2 and 3 above, so term is already (n-3k-9)!/(k!*6). */
        if ((k & 1U) == 0)          /* (-1)^k = 1 when k is even */
            sum += term;
        else                        /* (-1)^k = -1 when k is odd */
            sum -= term;
    }

    return sum;
}
