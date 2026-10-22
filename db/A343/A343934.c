/*
 * A343934  Irregular triangle read by rows: row n gives the sequence of
 *          iterations of k - A006519(k), starting with k=n, until 0 is
 *          reached.
 * https://oeis.org/A343934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1024, u16 n <= 5120, u32 n <= 5120, u64 n <= 5120,
 *            u128 n <= 5120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth element of the irregular triangle read by rows, the row r
   consisting of r with its successive lowest set bits cleared. */

/* Returns the n‑th term of A343934.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below the offset – undefined */
    idx_t cumulative = 0;                /* sum of lengths of previous rows */
    idx_t r = 1;                          /* current row index */

    /* Find the row r such that the nth element lies in this row. */
    while (1) {
        idx_t len = (idx_t)__builtin_popcountll(r);   /* length = popcount(r) */
        if (cumulative + len >= n) break;
        cumulative += len;
        r++;
    }

    /* Position within the row: 0‑based index. */
    idx_t pos = n - cumulative - 1;

    /* The first element of the row is r itself. */
    seq_t val = (seq_t)r;
    if (pos == 0) return val;

    /* Clear the lowest 'pos' set bits of r to get the desired element. */
    idx_t temp = r;                 /* working copy to iteratively clear bits */
    for (idx_t i = 0; i < pos; ++i) {
        /* lowest set bit in temp */
        uint64_t low = temp & -temp;
        val &= ~((seq_t)low);       /* clear that bit from the value */
        temp ^= low;                /* remove that bit from temp for next step */
    }

    return val;
}
