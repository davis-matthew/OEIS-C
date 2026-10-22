/*
 * A354785  Numbers of the form 3*2^k or 9*2^k.
 * https://oeis.org/A354785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers of the form 3*2^k or 9*2^k
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* first term */
    seq_t cur = 3;
    /* next values coming from the two geometric progressions */
    seq_t next1 = 3;    /* 3*2^k   k = 0 */
    seq_t next2 = 9;    /* 9*2^k   k = 0 */
    /* advance next1 for future */
    next1 = next1 * 2;

    idx_t count = 1;
    while (count < n) {
        if (next1 <= next2) {
            cur   = next1;
            next1 = next1 * 2;
        } else {
            cur   = next2;
            next2 = next2 * 2;
        }
        count++;
    }
    return cur;
}
