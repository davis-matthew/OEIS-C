/*
 * A343907  a(n) is the least s > 1 for which it is possible to place s
 *          nonattacking range-n leprechauns on an s X s board.
 * https://oeis.org/A343907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n odd)?(n+1)^2 : (n^2 + 2*n + ( (n%8==4||n%8==6) ? 4 : 2
 *            ))
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result;
    if ((n & 1ULL) == 1ULL) {
        seq_t np1 = (seq_t)(n + 1);
        result = np1 * np1;
    } else {
        seq_t nn = (seq_t)n;
        result = nn * nn + (seq_t)2 * nn;
        uint64_t mod8 = n % 8ULL;
        result += (mod8 == 4ULL || mod8 == 6ULL) ? (seq_t)4 : (seq_t)2;
    }
    return result;
}
