/*
 * A355986  a(n) = Sum_{k=1..n} floor(n/k)!.
 * https://oeis.org/A355986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum  = (seq_t)0;   /* Accumulator for the result */
    seq_t fact = (seq_t)1;   /* Holds m! during the loop */

    for (idx_t m = 1; m <= n; ++m) {
        fact *= (seq_t)m;                      /* Increment factorial: m! = (m-1)! * m */

        /* Count how many k satisfy floor(n/k) == m */
        idx_t high = n / m;                    /* Largest k giving floor(n/k) == m */
        idx_t low  = n / (m + 1) + 1;          /* Smallest k giving floor(n/k) == m */
        idx_t cnt  = 0;
        if (high >= low) {
            cnt = high - low + 1;
        }

        if (cnt != 0) {
            sum += fact * (seq_t)cnt;          /* Add m! * count to the sum */
        }
    }

    return sum;
}
