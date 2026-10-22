/*
 * A346310  Positions of words in A076478 such that #0's - #1's is even.
 * https://oeis.org/A346310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = position of the n-th even‑length binary word in the
 *            dictionary order of all binary words. The index starts at 1
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* pow = 2^L where L is the smallest even length such that the
       cumulative count of even‑length words up to that length is at least n. */
    seq_t pow = (seq_t)1 << 2;      /* 2^2 = 4 */
    seq_t cur = (seq_t)0;           /* sum of 2^{2i} for i < m  */

    while (cur + pow < (seq_t)n) {
        cur += pow;                 /* add previous block of length L-2 */
        pow <<= 2;                 /* move to next even length: multiply by 4 */
    }

    /* Now pow = 2^L (L is even), cur = sum_{i=1}^{m-1} 2^{2i} */
    seq_t offset = (seq_t)n - cur - (seq_t)1;   /* 0‑based position within length L block */
    return pow - (seq_t)1 + offset;             /* overall position in the full list */
}
