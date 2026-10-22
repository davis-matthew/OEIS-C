/*
 * A342194  Number of strict compositions of n with equal differences, or
 *          strict arithmetic progressions summing to n.
 * https://oeis.org/A342194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (# of strict arithmetic progressions of positive
 *            integers with positive common difference summing to n) + 1 (a(0)
 *            = 1). Implementation loops over the length of the progression
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;        /* offset a(0) = 1 */

    seq_t count_dpos = (seq_t)0;

    for (idx_t k = 2; ; ++k) {
        /* Minimum possible sum for this length (a1 = 1, d = 1). */
        seq_t minsum = (seq_t)k * (k + 1) / 2;
        if (minsum > (seq_t)n)
            break;               /* no larger k can satisfy the sum */

        seq_t B = (seq_t)k * (k - 1) / 2;   /* coefficient of d in the sum formula */

        /* Largest positive difference that keeps all terms >= 1. */
        idx_t max_d = (n > k) ? (idx_t)((seq_t)(n - k) / B) : 0;
        if (max_d == 0)
            continue;

        for (idx_t d = 1; d <= max_d; ++d) {
            /* (n - d*B) must be divisible by k to give an integer starting term a1 >= 1. */
            if (((seq_t)n - (seq_t)d * B) % (seq_t)k == 0)
                ++count_dpos;
        }
    }

    return count_dpos * (seq_t)2 + (seq_t)1;   /* add the single‑term composition */
}
