/*
 * A347710  Number of compositions (ordered partitions) of n into at most 3
 *          squares.
 * https://oeis.org/A347710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of compositions of n into at most 3 squares
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t count = 0;

    /* single square */
    for (idx_t a = 1; ; ++a) {
        idx_t sqA = a * a;
        if (sqA > n)
            break;
        if (sqA == n)
            ++count;

        /* pairs of squares */
        for (idx_t b = 1; ; ++b) {
            idx_t sqB = b * b;
            if (sqA + sqB > n)
                break;
            if (sqA + sqB == n)
                ++count;

            /* triples of squares */
            for (idx_t c = 1; ; ++c) {
                idx_t sum = sqA + sqB + c * c;
                if (sum > n)
                    break;
                if (sum == n)
                    ++count;
            }
        }
    }
    return count;
}
