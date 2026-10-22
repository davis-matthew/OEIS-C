/*
 * A308107  Sum of the smallest side lengths of all integer-sided scalene
 *          triangles with perimeter n.
 * https://oeis.org/A308107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{a=1..⌊(n-3)/3⌋} a *
 *   max( 0,
 *       ⌊(n-a-1)/2⌋
 *       - max( a+1, ⌊n/2⌋-a+1 )
 *       + 1 )
 * This computes the sum of the smallest side lengths of all
 * integer‑sided scalene triangles with perimeter n.
 */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* For n < 9 no scalene triangle exists */
    if (n < 9) return sum;

    /* a ranges from 1 to ⌊(n-3)/3⌋ */
    idx_t a_max = (n - 3) / 3;
    for (idx_t a = 1; a <= a_max; ++a)
    {
        /* lower bound on b: max(a+1, ⌊n/2⌋-a+1) */
        idx_t half_n = n / 2;
        idx_t b_min1 = a + 1;
        idx_t b_min2 = half_n - a + 1;
        idx_t b_min = (b_min1 > b_min2) ? b_min1 : b_min2;

        /* upper bound on b: ⌊(n-a-1)/2⌋ */
        idx_t b_max = (n - a - 1) / 2;

        if (b_max >= b_min)
        {
            idx_t cnt = b_max - b_min + 1;
            sum += (seq_t)a * (seq_t)cnt;
        }
    }

    return sum;
}
