/*
 * A341551  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(6,n) (with n at least 4) missing three edges, where
 *          exactly two of the removed edges are incident to the same vertex
 *          in the 6-point set but none of the remov...
 * https://oeis.org/A341551
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 29791*63^(n-3) – 34890*31^(n-3) + 14673*15^(n-3) –
 *            2740*7^(n-3) + 211*3^(n-3) – 4 (offset 4)
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: fast exponentiation for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 4) return 0;          /* undefined for n < 4, but harmless */

    idx_t k = n - 3;              /* exponent */

    seq_t p3   = pow_seq(3,   k);
    seq_t p7   = pow_seq(7,   k);
    seq_t p15  = pow_seq(15,  k);
    seq_t p31  = pow_seq(31,  k);
    seq_t p63  = pow_seq(63,  k);

    seq_t result = 29791 * p63;   /* term1 */
    result -= 34890 * p31;        /* term2 */
    result += 14673 * p15;        /* term3 */
    result -= 2740  * p7;         /* term4 */
    result += 211   * p3;         /* term5 */
    result -= 4;                  /* constant */

    return result;
}
