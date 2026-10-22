/*
 * A340134  a(n+1) = a(n-2*a(n)) + 1, starting with a(1) = a(2) = 0.
 * https://oeis.org/A340134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(n-1)) + ((-1)^(n+floor(sqrt(n)))-1)/2
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes this value using integer arithmetic only. */

static inline idx_t int_sqrt_u64(idx_t n)
{
    /* Return the integer part of sqrt(n) without overflow. */
    if (n == 0)
        return 0;

    idx_t low  = 1;
    idx_t high = (n >> 1) + 1;   /* √n ≤ n/2+1 for n ≥ 1 */
    idx_t ans  = 1;

    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);

        if (mid > n / mid)    /* mid*mid would overflow or be too large */
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
    /* According to the offset, n is always ≥ 1 for valid calls. */
    idx_t s1 = int_sqrt_u64(n - 1);   /* floor(sqrt(n-1)) */
    idx_t s2 = int_sqrt_u64(n);       /* floor(sqrt(n))   */

    /* parity term: 1 if (n + s2) is odd, otherwise 0 */
    idx_t parity = (n + s2) & 1ULL;

    return (seq_t)(s1 - parity);
}
