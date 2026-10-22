/*
 * A385882  Values of v in the (1,3)-quartals (m,u,v,w) having m=1; i.e.,
 *          values of v for solutions to m^1 + u^3 = v^1 + w^3, in positive
 *          integers, with m<v, sorted by u, then v.
 * https://oeis.org/A385882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + u³ – (u – k)³, where u is the smallest integer with
 *            u(u–1)/2 ≥ n, and k = n – (u–1)(u–2)/2
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find the u block containing the n-th term. */
    idx_t u = 2;                  /* u starts from 2 (u=1 gives no solutions). */
    while (1) {
        idx_t count = u - 1;      /* number of v's for this u. */
        if (n <= count) {         /* n lies in this block. */
            idx_t k = n;          /* 1‑based index within the block. */
            idx_t w = u - k;      /* corresponding w (descending). */
            seq_t us = (seq_t)u;
            seq_t ws = (seq_t)w;
            seq_t us3 = us * us * us;  /* u³ */
            seq_t ws3 = ws * ws * ws;  /* w³ */
            return (seq_t)1 + us3 - ws3;
        }
        n -= count;                /* skip this whole block */
        u++;                       /* move to next u */
    }
}
