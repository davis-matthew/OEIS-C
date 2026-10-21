/*
 * A268240  Pascal's tetrahedron of trinomial coefficients (A046816) read mod
 *          2.
 * https://oeis.org/A268240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 1 if the ternary coordinates (i,j,k) of the * nth entry
 *            (ordered by total sum s, then i descending, * then j ascending)
 *            have no overlapping 1-bits in binary, * i.e. (i&j)==0 &&
 *            (i&k)==0 && (j&k)==0; otherwise a(n)=0
 * Range:     u8 n <= 5455, u16 n <= 5455, u32 n <= 5455, u64 n <= 5455,
 *            u128 n <= 5455, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the slice index s such that total entries before s+1 exceed n. */
    idx_t s = 0;
    while ((s + 3) * (s + 2) * (s + 1) / 6 <= n) {
        s++;
    }
    /* Number of entries with total sum < s. */
    idx_t count_before = (s + 2) * (s + 1) * s / 6;   /* (s+2 choose 3) */
    idx_t offset = n - count_before;                  /* 0 … (s+2 choose 2)-1 */

    /* Determine which i‑block the offset falls into.
     * Blocks are sized 1,2,3,…,s+1 (for i = s,s-1,…,0). */
    idx_t t = 0;          /* number of completed blocks before the target block */
    idx_t cum = 0;        /* cumulative elements of completed blocks */
    while (cum + (t + 1) <= offset) {
        cum += t + 1;
        t++;
    }

    /* Coordinates within the slice. */
    idx_t i = s - t;
    idx_t j = offset - cum;
    idx_t k = s - i - j;

    /* Parity test using bitwise carry‑free addition. */
    seq_t res = 0;
    if (((i & j) == 0) && ((i & k) == 0) && ((j & k) == 0)) {
        res = 1;
    }
    return res;
}
