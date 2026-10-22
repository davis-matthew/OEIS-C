/*
 * A370973  Nearest integer to e*n!.
 * https://oeis.org/A370973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to e*n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n >= 2, this equals S(n) = sum_{k=0}^n n!/k!.  
   For n = 0 and 1 an extra 2 or 1 is added to round up. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)3;           /* 1 + 2  */
    seq_t s = (seq_t)1;                    /* S(0)   */
    for (idx_t i = 0; i < n; ++i) {
        s = ((seq_t)(i + 1)) * s + (seq_t)1;   /* S(i+1) */
    }
    if (n == 1) s += (seq_t)1;            /* round up at n = 1 */
    return s;
}
