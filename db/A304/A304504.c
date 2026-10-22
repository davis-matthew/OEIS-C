/*
 * A304504  a(n) = 3*(3*n+1)*(9*n+8)/2.
 * https://oeis.org/A304504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(3*n+1)*(9*n+8)/2
 * Range:     u8 n <= 1, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 12, a(1) = 102, a(2) = 273, … as listed in OEIS entry. */
seq_t a(idx_t n) {
    /* cast n to seq_t once to avoid repeated cast */
    seq_t nn = n;
    /* compute (3*n+1) and (9*n+8) in seq_t */
    seq_t t1 = ((seq_t)3 * nn + 1);
    seq_t t2 = ((seq_t)9 * nn + 8);
    /* multiply, then scale by 3 and divide by 2 */
    seq_t res = t1 * t2;
    res *= 3;
    res /= 2;
    return res;
}
