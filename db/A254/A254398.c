/*
 * A254398  Final digits of A237424 in decimal representation.
 * https://oeis.org/A254398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254398: a(1)=1, thereafter a 4 followed by k sevens, for
   k=1,2,3,...   (blocks of length k + 1, obtained in order). */

seq_t a(idx_t n) {
    if (n == 1) { return (seq_t)1; }

    idx_t pos   = 2;   /* position of the first 4 */
    idx_t block = 1;   /* current run length of 7's */

    while (1) {
        /* 4 at the start of the block */
        if (pos == n) { return (seq_t)4; }

        /* the run of 7's of current block length */
        for (idx_t i = 0; i < block; ++i) {
            ++pos;                       /* move to next position */
            if (pos == n) { return (seq_t)7; }
        }

        /* prepare the next block: increment run length, move to its 4 */
        ++block;
        ++pos;
    }
}
