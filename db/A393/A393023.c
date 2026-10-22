/*
 * A393023  List points (x,y,z) having integer coordinates, sorted first by L
 *          = |x| + |y| + |z| and in case of ties, then by z and last by polar
 *          angle 0 <= phi < 2*Pi in a polar coordinate system. Sequence gives
 *          z-coordinates.
 * https://oeis.org/A393023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = z-coordinate of the n-th lattice point sorted by L = |x|
 *            + |y| + |z|, then by z, then by polar angle φ of (x,y)
 * Range:     i8 n <= 9757, i16 n <= 9757, i32 n <= 9757, i64 n <= 9757,
 *            i128 n <= 9757, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the z-value of the n-th point in the ordering described above. */
seq_t a(idx_t n) {
    idx_t cumulative = 0;          /* total points processed so far   */
    idx_t L = 0;                    /* current L value                  */

    /* Find the L that contains the n-th point. */
    while (1) {
        /* Number of points with |x|+|y|+|z| = L.  */
        idx_t points_at_L = 0;
        for (idx_t k = 0; k <= L; ++k) {
            idx_t t = L - k;                     /* |x| + |y| for this |z| = k  */
            idx_t num = (t == 0) ? 1 : 4 * t;    /* number of (x,y) pairs       */
            idx_t weight = (k == 0) ? 1 : 2;     /* z = 0 has 1 weight, else 2  */
            points_at_L += weight * num;
        }

        if (cumulative + points_at_L > n) {
            break;   /* the desired point is in this L layer */
        }
        cumulative += points_at_L;
        ++L;         /* go to the next larger L */
    }

    /* Offset within this L layer. */
    idx_t offset = n - cumulative;

    /* Walk through the z-values in ascending order. */
    for (long long z = -(long long)L; z <= (long long)L; ++z) {
        idx_t absz = (z < 0) ? (idx_t)(-z) : (idx_t)z;
        idx_t t = L - absz;                    /* |x|+|y| for this z */
        idx_t num = (t == 0) ? 1 : 4 * t;       /* how many points with this z */
        if (offset < num) {
            return (seq_t)z;
        }
        offset -= num;
    }

    /* Should never reach here. */
    return 0;
}
