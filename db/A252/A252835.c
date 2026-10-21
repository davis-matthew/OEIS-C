/*
 * A252835  Number of nX7 nonnegative integer arrays with upper left 0 and
 *          every value within 2 of its king move distance from the upper left
 *          and every value increasing by 0 or 1 with every step right,
 *          diagonally se or down.
 * https://oeis.org/A252835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1306624*n^2 - 17846996*n + 62638467 for n > 11; * for n
 *            <= 11 use the first eleven terms of the sequence
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 64, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t init_11[11] = {
    22ULL,     125ULL,    850ULL,   6211ULL,  42738ULL,
    244131ULL, 1056756ULL,3320837ULL,7822881ULL,14827629ULL,
    24422839ULL
};

seq_t a(idx_t n)
{
    if (n <= 11) {
        return init_11[n - 1];
    }
    seq_t sn = (seq_t)n;
    seq_t res = 1306624 * sn * sn - 17846996 * sn + 62638467;
    return res;
}
