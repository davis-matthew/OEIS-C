/*
 * A344318  Number of partitions of n into consecutive parts not divisible by
 *          3.
 * https://oeis.org/A344318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of intervals [L,R] (1 ≤ L ≤ R) such that the sum
 *            of the integers in the interval that are not divisible by 3
 *            equals n. The implementation uses a sliding‑window approach
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t L = 1;          /* left boundary of the current window   */
    idx_t R = 0;          /* right boundary of the current window  */
    idx_t curSum = 0;     /* sum of non‑multiples of 3 in [L,R]    */
    idx_t ans   = 0;      /* number of desired partitions          */

    while (L <= n)               /* possible starts never exceed n      */
    {
        /* Extend the window until its sum is at least n */
        while (curSum < n)
        {
            ++R;
            if (R % 3u != 0)
                curSum += R;
        }

        /* Count the window only when it gives a sum equal to n
           and L itself is not a multiple of 3
           (ensures each partition is counted once). */
        if (curSum == n && (L % 3u != 0))
            ++ans;

        /* Remove the leftmost element from the window */
        if (L % 3u != 0)
            curSum -= L;

        ++L;
    }

    return (seq_t)ans;
}
