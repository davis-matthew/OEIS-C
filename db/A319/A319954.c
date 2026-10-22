/*
 * A319954  Infinite word over {0,1,2} formed from list of binary words of
 *          lengths 0, 1, 2, etc., including empty word, each prefixed by a 2.
 * https://oeis.org/A319954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th symbol of the infinite word obtained by *
 *            concatenating, for each k ≥ 0, the word '2' followed by all
 *            binary * words of length k in increasing order. * Example of the
 *            first few blocks: * k = 0: 2 * k = 1: 20, 21 * k = 2: 200, 201,
 *            210, 211 * k = 3: 2000, 2001, 2010, 2011, 2100, 2...
 * Range:     u8 n <= 8439, u16 n <= 8439, u32 n <= 8439, u64 n <= 8439,
 *            u128 n <= 8439, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the block length k (k+1 symbols, 2^k blocks) that contains n. */
    idx_t cumulative = 0;    /* total length upto the previous block length */
    idx_t k = 0;
    while (1) {
        idx_t width  = k + 1;                     /* symbols per block */
        idx_t blocks = ((idx_t)1) << k;           /* 2^k blocks */
        idx_t blockLen = blocks * width;          /* total symbols from this group */

        /* If n lies in this group, stop. */
        if (n < cumulative + blockLen)
            break;

        cumulative += blockLen;
        k++;
    }

    /* Position of n inside the selected group. */
    idx_t offset       = n - cumulative;        /* 0‑based offset within the group */
    idx_t block_index  = offset / (k + 1);      /* index of the block within the group */
    idx_t pos_in_block = offset % (k + 1);      /* position within that block (0 == the leading 2) */

    if (pos_in_block == 0)
        return (seq_t)2;           /* leading 2 of every block */

    /* For positions 1..k, the symbol is the (pos‑1)-th bit of block_index,
     * where bits are ordered from most‑significant to least‑significant. */
    seq_t bit = (seq_t)((block_index >> (k - pos_in_block)) & 1);
    return bit;
}
