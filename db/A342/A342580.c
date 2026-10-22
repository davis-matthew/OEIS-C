/*
 * A342580  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(5,n) (with n at least 4) missing three edges, where all
 *          three removed edges are incident to the same vertex in the 5-point
 *          set.
 * https://oeis.org/A342580
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3375*31^(n-3) - 4747*15^(n-3) - 166*3^(n-3) +
 *            1534*7^(n-3) + 4 (offset 4)
 * Range:     u8 none, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < (idx_t)4)
        return (seq_t)0;
    idx_t k = n - (idx_t)3;

    seq_t p31 = pow_seq((seq_t)31, k);
    seq_t p15 = pow_seq((seq_t)15, k);
    seq_t p3  = pow_seq((seq_t)3,  k);
    seq_t p7  = pow_seq((seq_t)7,  k);

    seq_t value = (seq_t)3375 * p31;
    value -= (seq_t)4747 * p15;
    value -= (seq_t)166 * p3;
    value += (seq_t)1534 * p7;
    value += (seq_t)4;
    return value;
}
