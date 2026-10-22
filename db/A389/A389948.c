/*
 * A389948  Shell enumeration of the upper halfplane. Terms are interleaved x
 *          and y coordinates.
 * https://oeis.org/A389948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, changed
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Shell enumeration of the upper halfplane.
   Points are listed by Chebyshev distance shells (r = 0, 1, 2, …).
   For each shell r>0 we traverse the boundary clockwise, starting
   at the point with the same sign of x as r (right side when r is odd,
   left side when r is even), only visiting points with y ≥ 0.
   The sequence a(n) interleaves the x‐ and y‐coordinates:
   a(2k-1) = x_k, a(2k) = y_k.                      */
seq_t a(idx_t n)
{
    /* Convert to 0‑based point index */
    idx_t i = (n - 1) / 2;

    /* Find the shell radius r such that cumulative(r‑1) ≤ i < cumulative(r)   */
    idx_t r = 0;
    idx_t cum = 1;                     /* cumulative count up to radius r   */
    while (cum <= i) {
        ++r;
        /* cum = (r+1)*(2*r+1) ; use __int128 to avoid overflow for large r */
        unsigned __int128 tmp = (unsigned __int128)(r + 1) * (2 * r + 1);
        cum = (idx_t)tmp;
    }

    /* cumulative count up to radius r‑1 */
    unsigned __int128 prev_tmp = (unsigned __int128)r * (2 * r - 1);
    idx_t prev_cum = (idx_t)prev_tmp;
    idx_t offset = i - prev_cum;        /* 0‑based offset within shell r     */

    long long x = 0, y = 0;
    if (r == 0) {
        x = y = 0;
    } else {
        long long sign = (r & 1) ? 1LL : -1LL;   /* start side            */
        if (offset <= r) {                         /* left/right side      */
            x = sign * (long long)r;
            y = (long long)offset;
        } else if (offset <= r + 2 * r) {          /* top side             */
            long long t = (long long)(offset - (r + 1));
            if (sign == 1LL) {
                x = (long long)(r - 1 - t);
                y = (long long)r;
            } else {
                x = (long long)(-r + 1 + t);
                y = (long long)r;
            }
        } else {                                   /* bottom side          */
            long long t = (long long)(offset - (3 * r + 1));
            if (sign == 1LL) {
                x = (long long)(-r);
                y = (long long)(r - 1 - t);
            } else {
                x = (long long)(r);
                y = (long long)(r - 1 - t);
            }
        }
    }

    /* Interleave x and y */
    return (n & 1) ? (seq_t)x : (seq_t)y;
}
