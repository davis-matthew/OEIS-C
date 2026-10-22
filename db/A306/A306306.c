/*
 * A306306  Number of binary words of length n such that in every prefix and
 *          in every suffix the difference between the number of 1's and the
 *          number of 0's is in the interval [-2,3].
 * https://oeis.org/A306306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary words of length n such that in every
 *            prefix and in every suffix the difference between the number of
 *            1's and the number of 0's lies in the interval [-2,3]
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 76, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1; /* trivial empty word */

    /* bounds for each final sum s_end */
    static const int L[6] = {-2, -2, -2, -2, -1,  0};
    static const int R[6] = { 0,  1,  2,  3,  3,  3};
    static const int sz[6] = { 3,  4,  5,  6,  5,  4};

    /* DP arrays: cur and next, sized to maximum bound (6 elements) */
    seq_t cur[6][7] = {0};
    seq_t nxt[6][7] = {0};

    /* initial state: sum 0, valid for all bounds */
    for (int j = 0; j < 6; ++j) {
        int idx0 = 0 - L[j];          /* index of value 0 in this bound */
        cur[j][idx0] = 1;
    }

    /* iterate over word positions */
    for (idx_t step = 1; step <= n; ++step) {
        for (int j = 0; j < 6; ++j) {
            for (int i = 0; i < sz[j]; ++i) {
                seq_t val = 0;
                if (i > 0)      val += cur[j][i - 1];   /* from value-1 */
                if (i + 1 < sz[j]) val += cur[j][i + 1]; /* from value+1 */
                nxt[j][i] = val;
            }
        }
        /* copy next to current for next iteration */
        for (int j = 0; j < 6; ++j) {
            for (int i = 0; i < sz[j]; ++i) {
                cur[j][i] = nxt[j][i];
            }
        }
    }

    /* sum counts for each possible final sum s_end */
    seq_t result = 0;
    /* mapping from s_end in [-2,3] to its bound index */
    static const int boundIndex[6] = {0, 1, 2, 3, 4, 5};

    for (int s = -2; s <= 3; ++s) {
        int b = boundIndex[s + 2];
        int idx = s - L[b];
        result += cur[b][idx];
    }

    return result;
}
