/*
 * A346309  Positions of words in A076478 such that number of 0's - number of
 *          1's is odd.
 * https://oeis.org/A346309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A053754(n+1) - 1
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Off‑set is 1; return 0 for a(0) for completeness. */
        return (seq_t)0;
    }

    /* Convert the 64‑bit index to the big‑integer type once. */
    seq_t idx = (seq_t)n;

    /* Cumulative size of all previous blocks (A053754 values). */
    seq_t cum  = (seq_t)0;

    /* Size of the current block: 2^(2k-1).  Starts at 2. */
    seq_t size = (seq_t)2;

    /* Advance block by block until the desired index lies in the current one. */
    while (cum + size < idx) {
        cum  += size;   /* add previous block */
        size *= (seq_t)4;   /* next block is four times larger */
    }

    /* First value in the current block (a(n) is 0‑based within the block). */
    seq_t start  = size - (seq_t)1;

    /* Position inside the block (0‑based). */
    seq_t offset = idx - cum - (seq_t)1;

    /* The required term. */
    return start + offset;
}
