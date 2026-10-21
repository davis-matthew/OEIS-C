/*
 * A279891  Triangle read by rows, T(n,k) = 2*n, with n>=k>=0.
 * https://oeis.org/A279891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2 * floor((sqrt(1+8*n)-1)/2)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the largest integer t such that t*(t+1)/2 <= n */
    idx_t low = 0, high = n, mid, best = 0;

    while (low <= high) {
        /* Avoid overflow when computing mid + high */
        mid = low + ((high - low) >> 1);

        /* Compute mid*(mid+1)/2 using 128‑bit integer to avoid overflow */
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        prod >>= 1;  /* divide by 2 */

        if (prod <= n) {
            best = mid;          /* mid satisfies the inequality */
            low = mid + 1;       /* try a larger value */
        } else if (mid == 0) {
            break;               /* can't go smaller */
        } else {
            high = mid - 1;      /* try a smaller value */
        }
    }

    /* Return 2*best as the required seq_t value */
    seq_t res = (seq_t)best;
    res <<= 1;   /* multiply by 2 */
    return res;
}
