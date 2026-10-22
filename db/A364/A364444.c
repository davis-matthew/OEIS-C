/*
 * A364444  a(n) is the number of integers k with n^2 < k < (n+1)^2 that are
 *          the sum of no more than 3 squares, counting multiple
 *          representations only once.
 * https://oeis.org/A364444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - (# of integers k with n^2 < k < (n+1)^2 that are of
 *            the form 4^a * (8*b + 7)). Legendre's three-square theorem says
 *            a number is representable as the sum of at most three squares
 *            iff it is not of that form. Thus a(n) counts such numbers among
 *            the 2*n integers between consecutive squares
 * Range:     u8 n <= 14, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* no numbers between 0² and 1² */

    seq_t lo = (seq_t)n * (seq_t)n;      /* n² */
    seq_t hi = (seq_t)(n + 1) * (seq_t)(n + 1); /* (n+1)² */
    seq_t total = (seq_t)2 * n;          /* there are 2n numbers in the interval */
    seq_t bad  = 0;                      /* count of numbers of the form 4^a(8b+7) */

    seq_t pow4 = (seq_t)1;               /* 4^a */

    /* Iterate over all powers of 4 that are not larger than (n+1)² */
    while (pow4 <= hi)
    {
        /* Lower bound of t = 8b + 7: lo / pow4 < t */
        seq_t t_min1 = lo / pow4 + 1;          /* first integer t > lo / pow4 */

        /* Ensure t is at least 7 (since 8b+7 ≥ 7) */
        seq_t start = t_min1;
        if (start < 7) start = 7;

        /* First t in the range that is ≡ 7 (mod 8) */
        seq_t mod = (start >= 7) ? (start - 7) % 8 : 0;
        seq_t first_t = start + (mod ? (8 - mod) : 0);

        /* Upper bound of t: ceil(hi / pow4) - 1  (strict inequality) */
        seq_t ceil_div = (hi + pow4 - 1) / pow4;  /* ceil(hi / pow4) */
        seq_t t_last   = ceil_div - 1;            /* largest integer < hi / pow4 */

        /* Last t in the range that is ≡ 7 (mod 8) */
        seq_t mod2 = (t_last >= 7) ? (t_last - 7) % 8 : 0;
        seq_t last_t = t_last - mod2;

        if (first_t <= last_t)
            bad += (last_t - first_t) / 8 + 1;    /* count of t ≡ 7 (mod 8) */

        /* Prepare next power of 4, avoid overflow */
        if (pow4 > hi / 4) break;
        pow4 = pow4 * 4;
    }

    return total - bad;
}
