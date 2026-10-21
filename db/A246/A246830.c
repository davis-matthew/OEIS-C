/*
 * A246830  T(n,k) is the concatenation of n-k and n+k in binary; triangle
 *          T(n,k), n>=0, 0<=k<=n, read by rows.
 * https://oeis.org/A246830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base, look, nice
 * Formula:   a(n) = concat_binary( i - j , i + j ) where i = floor( (sqrt(8*n
 *            + 1) - 1) / 2 ) j = n - i*(i + 1)/2 The concatenation uses the
 *            binary representations of the two operands without leading
 *            zeros; for zero the representation is a single bit “0”. The
 *            function is row‑wise traversal of the triangle T(n...
 * Range:     u8 n <= 81, u16 n <= 8255, u32 n <= 8255, u64 n <= 8255,
 *            u128 n <= 8255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static /* small helper: integer square root for 64‑bit unsigned numbers */
idx_t isqrt_u64(idx_t x) {
    /* CORDIC‑like bitwise algorithm (fast, no floating point) */
    idx_t r = 0;
    idx_t bit = 1ULL << 62;          /* highest power of 4 ≤ 2^64 */
    while (bit > x) bit >>= 2;
    for (; bit != 0; bit >>= 2) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
    }
    return r;
}

/* compute the bit length of a positive or zero seq_t value
   without using any library routine */
static unsigned bitlen_seq_t(seq_t v) {
    unsigned len = 1;           /* zero has length 1 */
    seq_t tmp = v;
    while (tmp > 1) {
        tmp >>= 1;
        ++len;
    }
    return len;
}

/* concatenate the binary representations of x and y.
   For a zero operand the representation is a single bit “0”. */
static seq_t concat_bin(seq_t x, seq_t y) {
    unsigned len_y = bitlen_seq_t(y);
    return (x << (unsigned long long)len_y) | y;
}

seq_t a(idx_t n) {
    /* row index i */
    idx_t row_index = (isqrt_u64(8ULL * n + 1ULL) - 1ULL) / 2ULL;

    /* start index of this row */
    idx_t prev_sum = row_index * (row_index + 1ULL) / 2ULL;

    /* column index j within the row */
    idx_t j = n - prev_sum;

    /* compute n-k and n+k */
    seq_t val1 = (seq_t)(row_index - j);   /* n - k */
    seq_t val2 = (seq_t)(row_index + j);   /* n + k */

    return concat_bin(val1, val2);
}
