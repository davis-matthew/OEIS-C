/*
 * A241662  Numbers of the form m * 10^k where gcd(10, m) = 1 and k >= 0 and m
 *          > 0.
 * https://oeis.org/A241662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A241662: the n‑th positive integer whose exponents of 2 and 5 are equal.
 *  Equivalently, numbers of the form m*10^k with gcd(10,m)=1, k≥0.
 *  The function scans integers in increasing order and counts those with v2(n)=v5(n).
 *  Complexity is O(n) with a small constant; suitable for n up to ~10^4. */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0 */

    idx_t count = 0;
    for (seq_t num = 1;; ++num)   /* iterate over all positive integers */
    {
        /* count exponent of 2 */
        idx_t v2 = 0;
        seq_t t = num;
        while (t % 2 == 0) { t /= 2; ++v2; }

        /* count exponent of 5 */
        idx_t v5 = 0;
        t = num;
        while (t % 5 == 0) { t /= 5; ++v5; }

        if (v2 == v5)             /* satisfies the condition */
        {
            ++count;
            if (count == n) return num;
        }
    }
}
