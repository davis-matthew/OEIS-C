/*
 * A359216  X-coordinates of a point moving in a counterclockwise undulating
 *          spiral in a square grid.
 * https://oeis.org/A359216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, walk
 * Formula:   a(n) = B(ceil(n/2)) where B(m) is the sequence A329116 given by
 *            t = floor(sqrt(m)) j = m - t*t if t == 0 -> B(m) = 0 else if t
 *            odd -> B(m) = t - j else -> B(m) = -t + j
 * Range:     i8 n <= 9649, i16 n <= 9649, i32 n <= 9649, i64 n <= 9649,
 *            i128 n <= 9649, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* ceil(n/2) : shift right by 1 after adding 1 */
    idx_t m = (n + 1) >> 1;            /* argument of B */

    /* compute t = floor(sqrt(m)) with a small integer loop */
    idx_t t = 0;
    while ((t + 1) * (t + 1) <= m) {
        ++t;
    }

    idx_t j = m - t * t;               /* position inside the current cycle */

    seq_t val;
    if (t == 0) {
        val = 0;
    } else if (t & 1U) {               /* t is odd  -> descending from +t to -t */
        val = (seq_t)t - (seq_t)j;
    } else {                           /* t is even -> ascending from -t to +t */
        val = -(seq_t)t + (seq_t)j;
    }
    return val;
}
