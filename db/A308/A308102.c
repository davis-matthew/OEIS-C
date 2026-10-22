/*
 * A308102  Sum of the perimeters of all integer-sided scalene triangles with
 *          perimeter n.
 * https://oeis.org/A308102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (# of integer‑sided scalene triangles with perimeter
 *            n). Computed by enumerating integer side lengths a<b<c such that
 *            a+b+c = n and a+b > c
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* The maximum possible value of the smallest side a is floor((n-1)/3). */
    idx_t max_a = (n - 1) / 3;
    for (idx_t a = 1; a <= max_a; ++a) {
        /* Lower bound for b: must satisfy a < b and a + b > n/2. */
        idx_t lower = a + 1;
        /* floor((n - 2*a)/2) + 1 implements floor(n/2 - a) + 1 efficiently. */
        idx_t tmp = (n - 2 * a) / 2 + 1;
        if (tmp > lower) lower = tmp;

        /* Upper bound for b: must satisfy b < (n - a)/2 so that c > b. */
        idx_t upper = (n - a - 1) / 2;

        if (lower <= upper) {
            count += (seq_t)(upper - lower + 1);
        }
    }
    return (seq_t)n * count;
}
