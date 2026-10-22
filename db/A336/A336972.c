/*
 * A336972  Sum of the smallest two side lengths of all distinct integer-sided
 *          triangles with perimeter n.
 * https://oeis.org/A336972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A336972: Sum of the smallest two side lengths of all distinct integer-sided triangles with perimeter n */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t a_side = 1; a_side <= n / 3; ++a_side)
    {
        /* lower bound for b: must satisfy a_side + b > c, i.e. 2*a_side + 2*b > n  */
        idx_t low = (n - 2 * a_side) >> 1;   /* floor((n-2*a)/2) */
        low += 1;                            /* b > (n-2*a)/2  */
        if (low < a_side) low = a_side;      /* also need b >= a_side */

        idx_t high = (n - a_side) >> 1;      /* b <= (n-a)/2  */
        if (low > high) continue;

        idx_t t = high - low + 1;            /* number of valid b's */

        /* contribution for this a:  sum_{b=low}^{high} (a_side + b)
           = t*a_side + t*low + t*(t-1)/2                         */
        sum += (seq_t)t * (seq_t)a_side
             + (seq_t)t * (seq_t)low
             + (seq_t)t * (t - 1) / 2;
    }

    return sum;
}
