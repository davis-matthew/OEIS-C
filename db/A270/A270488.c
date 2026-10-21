/*
 * A270488  Number of ordered ways to write n = x^2 + y*(y+1) + z*(z^2+1),
 *          where x, y and z are nonnegative integers.
 * https://oeis.org/A270488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 628, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of ordered triples (x, y, z) of nonnegative integers
 *                such that n = x^2 + y*(y+1) + z*(z^2 + 1)
 */
seq_t a(idx_t n)
{
    /* convert the index to the working type */
    seq_t N = (seq_t)n;
    seq_t count = (seq_t)0;

    /* loop over all possible x */
    for (idx_t x = 0; ; ++x) {
        seq_t xsq = (seq_t)x * (seq_t)x;
        if (xsq > N) break;
        seq_t rem1 = N - xsq;

        /* loop over all possible y for this x */
        for (idx_t y = 0; ; ++y) {
            seq_t ty = (seq_t)y * ((seq_t)y + (seq_t)1);
            if (ty > rem1) break;
            seq_t rem2 = rem1 - ty;

            /* loop over all possible z for this (x, y) pair */
            for (idx_t z = 0; ; ++z) {
                /* compute z*(z^2+1) */
                seq_t zsq = (seq_t)z * (seq_t)z;
                seq_t tz = (seq_t)z * (zsq + (seq_t)1);
                if (tz > rem2) break;
                if (tz == rem2) ++count;
            }
        }
    }

    return count;
}
