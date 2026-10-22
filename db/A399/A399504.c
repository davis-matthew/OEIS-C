/*
 * A399504  b - c - d where b >= c >= d >= 0 ordered by b then c then d.
 * https://oeis.org/A399504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, easy
 * Formula:   not stated
 * Range:     i8 n <= 8557, i16 n <= 8557, i32 n <= 8557, i64 n <= 8557,
 *            i128 n <= 8557, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A399504:
 * a(n) = b - c - d, where (b,c,d) is the n-th triple with
 * b >= c >= d >= 0, enumerated lexicographically by b, then c, then d. */

seq_t a(idx_t n)
{
    /* Find the value of b such that the n-th triple lies in the b‑block. */
    __int128 cumulative = 0;   /* cumulative triples processed so far, as __int128 */
    idx_t     b = 0;
    for (;; ++b) {
        __int128 count_b = (__int128)(b + 1) * (b + 2) / 2;  /* triples with this b */
        if (cumulative + count_b > n)                      /* stop when n is inside this block */
            break;
        cumulative += count_b;
    }

    /* Position of n inside the block for the found b. */
    idx_t p = (idx_t)(n - (idx_t)cumulative);   /* 0‑based index within block b */

    /* Find c such that p is in the c‑group inside the b‑block. */
    idx_t cum_c = 0;
    idx_t c = 0;
    for (;; ++c) {
        idx_t count_c = (idx_t)(c + 1);            /* triples with this c (d goes 0..c) */
        if (cum_c + count_c > p)
            break;
        cum_c += count_c;
    }

    /* d is the offset inside the chosen c‑group. */
    idx_t d = p - cum_c;   /* 0 <= d <= c */

    /* Return the computed value.  The result is always within seq_t range. */
    return (seq_t)b - (seq_t)c - (seq_t)d;
}
