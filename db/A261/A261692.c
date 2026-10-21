/*
 * A261692  Number of "ON" cells after n-th stage in a cellular automaton in a
 *          90-degree wedge on the square grid. (See Comments lines for
 *          definition.)
 * https://oeis.org/A261692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=0; for n>0 let h=floor(log2 n). Then * a(n)= (4^h-1)/3 +
 *            (n-2^h+1) * (2^{h+1}-(n-2^h+1))
 * Range:     u8 n <= 21, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* floor(log2 n) */
    idx_t h = 0;
    for (idx_t t = n; t >>= 1; ++h);

    /* L = 2^h */
    unsigned long long L = 1ULL << h;

    /* b = n - 2^h + 1  (size of current block) */
    seq_t b = (seq_t)(n - L + 1);

    seq_t one = (seq_t)1;

    /* diff_start = 2^{h+1} - 1 */
    seq_t diff_start = (one << (h + 1)) - one;

    /* sum over full previous blocks */
    seq_t D;
    if (h == 0)
        D = (seq_t)0;
    else
        D = ((one << (2 * h)) - one) / (seq_t)3;   /* (4^h-1)/3 */

    /* partial sum within the current block */
    seq_t part = b * (diff_start - b + one);

    return D + part;
}
