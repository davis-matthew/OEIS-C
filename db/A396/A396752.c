/*
 * A396752  a(n) = n * floor(sqrt(n)).
 * https://oeis.org/A396752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * floor(sqrt(n))
 * Range:     u8 n <= 42, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(sqrt(n)) via binary search. */
    idx_t left  = 0;
    idx_t right = n;
    idx_t best  = 0;

    while (left <= right)
    {
        /* mid = (left + right) / 2  without overflow */
        idx_t mid = left + ((right - left) >> 1);

        /* use 128–bit arithmetic to avoid overflow in mid*mid */
        unsigned __int128 sq = (unsigned __int128)mid * mid;

        if (sq <= n)
        {
            best = mid;
            left = mid + 1;
        }
        else
        {
            if (mid == 0) break;   /* avoid underflow */
            right = mid - 1;
        }
    }

    return (seq_t)n * (seq_t)best;
}
