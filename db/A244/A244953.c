/*
 * A244953  a(n) = Sum_{i=0..n} (-i mod 4).
 * https://oeis.org/A244953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * floor((n+1)/4) + {0,0,3,5}[(n+1) mod 4]
 * Range:     u8 n <= 169, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      where partial[0]=0, partial[1]=0, partial[2]=3, partial[3]=5   */
seq_t a(idx_t n) {
    /* number of complete 4‑term blocks contained in the first n+1 terms   */
    idx_t cycles = n / 4;
    /* if n mod 4 == 3 then (n+1) crosses a new block boundary */
    if ((n & 3) == 3) {
        cycles++;
    }
    /* remainder of (n+1) modulo 4                                */
    idx_t r = ((n & 3) + 1) & 3;

    /* partial sums for 0..3 remainders (in seq_t)                  */
    static const seq_t partial[4] = {0, 0, 3, 5};

    /* assemble the result using seq_t arithmetic only               */
    return (seq_t)6 * (seq_t)cycles + partial[r];
}
