/*
 * A373733  Triangular numbers with tetrahedral indices: a(n) = (1/72) *
 *          (n*(n+1)*(n+2)*(n+3)*(n^2+2)).
 * https://oeis.org/A373733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/72) * (n*(n+1)*(n+2)*(n+3)*(n^2+2))
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 81, u64 n <= 3314,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t denom = 72;
    seq_t part[5];

    part[0] = sn;                    /* n */
    part[1] = sn + 1;                /* n+1 */
    part[2] = sn + 2;                /* n+2 */
    part[3] = sn + 3;                /* n+3 */
    part[4] = sn * sn + 2;           /* n^2+2 */

    /* Cancel common 2 and 3 factors with the denominator.
       After this loop denom will be 1. */
    for (int i = 0; i < 5; ++i) {
        seq_t v = part[i];

        while ((denom % 2 == 0) && (v % 2 == 0)) {
            v /= 2;
            denom /= 2;
        }
        while ((denom % 3 == 0) && (v % 3 == 0)) {
            v /= 3;
            denom /= 3;
        }
        part[i] = v;
    }

    seq_t result = 1;
    for (int i = 0; i < 5; ++i)
        result *= part[i];

    /* denom should be 1 (full cancellation); ignore otherwise. */
    return result;
}
