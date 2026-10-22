/*
 * A359042  Sum of partial sums of the n-th composition in standard order
 *          (A066099).
 * https://oeis.org/A359042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum of partial sums of the n-th composition in standard
 *            order (A066099)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Positions of 1's in the reversed binary expansion of n, plus a leading 0 */
    idx_t pos[65];     /* maximum of 64 bits + the leading zero */
    int posCnt = 1;
    pos[0] = 0;

    idx_t tmp = n;
    int bit = 1;
    while (tmp != 0) {
        if (tmp & 1) {
            pos[posCnt++] = bit;
        }
        tmp >>= 1;
        ++bit;
    }

    /* Compute the sum of partial sums of the composition parts.
       The parts are the reversed differences of consecutive positions. */
    seq_t cum = 0;
    seq_t sum = 0;
    int diffCnt = posCnt - 1;
    for (int i = diffCnt - 1; i >= 0; --i) {
        seq_t part = pos[i + 1] - pos[i];
        cum += part;
        sum += cum;
    }

    return sum;
}
