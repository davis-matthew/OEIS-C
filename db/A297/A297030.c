/*
 * A297030  Number of pieces in the list d(m), d(m-1), ..., d(0) of base-2
 *          digits of n; see Comments.
 * https://oeis.org/A297030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of pieces in the binary digit sequence of n (OEIS
 *            A297030)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n == 1 has no adjacent digit, hence 0 pieces */
    if (n == 1)
        return 0;

    /* determine the number of bits */
    idx_t tmp = n;
    int bits = 0;
    while (tmp) {
        bits++;
        tmp >>= 1;
    }

    /* get the most significant bit as the first digit */
    int prev_bit = (int)((n >> (bits - 1)) & 1ULL);
    seq_t count = 1;           /* at least one piece */
    int prev_type = 0;         /* 0 = flat, -1 = decreasing, 1 = increasing */
    int first_diff_done = 0;   /* flag for the first comparison */

    /* iterate over all following bits from msb to lsb */
    for (int i = bits - 2; i >= 0; --i) {
        int bit = (int)((n >> i) & 1ULL);
        int diff = bit - prev_bit;
        int new_type;

        if (diff == 0)
            new_type = 0;          /* flat */
        else if (diff < 0)
            new_type = -1;         /* decreasing */
        else
            new_type = 1;          /* increasing */

        if (!first_diff_done) {
            /* establish the first trend, no count increment */
            prev_type = new_type;
            first_diff_done = 1;
        } else if (new_type != prev_type) {
            /* trend change -> new piece */
            count++;
            prev_type = new_type;
        }

        prev_bit = bit;
    }

    return count;
}
