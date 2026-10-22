/*
 * A342218  The n-th and a(n)-th points of the Peano curve (A163528, A163529)
 *          are symmetrical with respect to the line X=Y.
 * https://oeis.org/A342218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) is the index of the point symmetric to point n about line
 *            X=Y in the Peano Curve (A163528, A163529)
 * Range:     u8 n <= 80, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const int base_coords[9][2] = {
    {0, 0}, {0, 1}, {0, 2},
    {1, 2}, {1, 1}, {1, 0},
    {2, 0}, {2, 1}, {2, 2}
};
static const int rev_index[3][3] = {
    {0, 1, 2},
    {5, 4, 3},
    {6, 7, 8}
};

/* Forward: compute (x,y) coordinates of index n in the Peano curve. */
static void coords_from_n(idx_t n, seq_t *px, seq_t *py) {
    if (n < 9) {
        *px = (seq_t)base_coords[n][0];
        *py = (seq_t)base_coords[n][1];
        return;
    }
    idx_t high = n / 9;
    idx_t low  = n % 9;
    seq_t xh, yh;
    coords_from_n(high, &xh, &yh);
    *px = xh * (seq_t)3 + (seq_t)base_coords[low][0];
    *py = yh * (seq_t)3 + (seq_t)base_coords[low][1];
}

/* Inverse: compute the index whose coordinates are (x,y). */
static idx_t index_from_coords(seq_t x, seq_t y) {
    if ( (x < (seq_t)3) && (y < (seq_t)3) ) {
        /* Base case: small coordinates. */
        int low = rev_index[(int)x][(int)y];
        return (idx_t)low;
    }
    /* Determine the low sub-square (in 0..8) by taking coordinates modulo 3. */
    int low = rev_index[(int)(x % (seq_t)3)][(int)(y % (seq_t)3)];
    seq_t base_x_low = (seq_t)base_coords[low][0];
    seq_t base_y_low = (seq_t)base_coords[low][1];
    seq_t xh = (x - base_x_low) / (seq_t)3;
    seq_t yh = (y - base_y_low) / (seq_t)3;
    idx_t high = index_from_coords(xh, yh);
    return high * (idx_t)9 + (idx_t)low;
}

seq_t a(idx_t n) {
    seq_t x, y;
    coords_from_n(n, &x, &y);
    idx_t partner = index_from_coords(y, x);
    return (seq_t)partner;
}
