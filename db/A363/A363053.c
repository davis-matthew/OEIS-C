/*
 * A363053  Sum of divisors of 5*n-4 of form 5*k+3.
 * https://oeis.org/A363053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ{ d | (5*n-4), d ≡ 3 (mod 5) } d
 * Range:     u8 n <= 51, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root for 64–bit unsigned integers */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) { ans = 0; low = 1; continue; }
        if (mid > n / mid)            /* mid*mid would overflow or be > n */
            high = mid - 1;
        else
        {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* compute m = 5*n - 4, avoiding overflow by using seq_t first */
    seq_t temp = (seq_t)5 * (seq_t)n - 4;
    idx_t m = (idx_t)temp;         /* m fits in idx_t for the considered test range */

    seq_t sum = 0;
    idx_t limit = isqrt_u64(m);

    for (idx_t d = 1; d <= limit; ++d)
    {
        if (m % d == 0)
        {
            idx_t d_pair = m / d;
            if (d % 5 == 3)
                sum += (seq_t)d;
            if (d_pair != d && d_pair % 5 == 3)
                sum += (seq_t)d_pair;
        }
    }
    return sum;
}
