/*
 * A295320  Sum of the products of the smaller and larger parts of the
 *          partitions of n into two distinct parts with the larger part odd.
 * https://oeis.org/A295320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..⌊(n-1)/2⌋} i * (n-i) * ((n-i) mod 2)
 * Range:     u8 n <= 18, u16 n <= 116, u32 n <= 4688, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   i.e. sum i*(n-i) over all i such that n-i is odd */

seq_t a(idx_t n) {
    seq_t sum = 0;
    /* Max i is floor((n-1)/2).  For n <= 1 the loop is skipped. */
    idx_t imax = (n - 1) / 2;
    for (idx_t i = 1; i <= imax; ++i) {
        idx_t l = n - i;          /* larger part of the partition */
        if (l & 1) {              /* include only if l is odd */
            sum += (seq_t)i * (seq_t)l;
        }
    }
    return sum;
}
