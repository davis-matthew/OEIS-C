/*
 * A280514  Index sequence of the reverse block-fractal sequence A003849.
 * https://oeis.org/A280514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 608, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A280514 : the index sequence of the reverse block‑fractal
   sequence A003849.  The n-th value is found by locating the block
   (whose length is a Fibonacci number) that contains n and then
   returning the decreasing value in that block.
   Algorithm: iteratively subtract Fibonacci block lengths
   until n lies in the current block.  Complexity O(logφ(n)). */
seq_t a(idx_t n) {
    /* n >= 1 */
    idx_t rem = n;               /* remaining position within the
                                   yet‑to‑be‑scanned part of the
                                   infinite sequence                      */
    seq_t f_prev = (seq_t)1;     /* F(1) */
    seq_t f_curr = (seq_t)1;     /* F(2) */

    for (;;) {
        seq_t block = f_curr;        /* block length = F(k+1) for
                                         current block k                           */
        if (rem <= (idx_t)block) {   /* n lies in this block          */
            idx_t offset = rem - 1;  /* zero‑based offset in block    */
            return block - offset;   /* decreasing sequence value    */
        }
        rem -= (idx_t)block;          /* skip this entire block        */
        /* advance to next Fibonacci number */
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;
    }
}
