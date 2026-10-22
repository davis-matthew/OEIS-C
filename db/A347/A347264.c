/*
 * A347264  Largest value in the 3x+1 sequence starting at n, divided by 4.
 * https://oeis.org/A347264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A056959(n)/4, where A056959(n) is the largest value in
 *            the 3x+1 sequence starting from n
 * Range:     u8 n <= 26, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == (idx_t)0) return 0;          /* sequence defined for n ≥ 1 */

    seq_t x   = (seq_t)n;                 /* current term */
    seq_t max = x;                        /* maximum seen so far */

    /* Run the Collatz iterations until we have seen the value 1
       with max ≥ 4.  When max is already ≥ 4 we are done, otherwise
       we must perform at least one more step to capture the 4 that
       follows 1. */
    do {
        if ((x & (seq_t)1) == (seq_t)0) {  /* even step */
            x = x / (seq_t)2;
        } else {                          /* odd step */
            x = (seq_t)3 * x + (seq_t)1;
        }
        if (x > max) max = x;
    } while (x != (seq_t)1 || max < (seq_t)4);

    return max / (seq_t)4;
}
