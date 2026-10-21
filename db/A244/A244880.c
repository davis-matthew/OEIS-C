/*
 * A244880  Number of magic labelings of the cycle-of-loops graph LOOP X C_8
 *          having magic sum n, where LOOP is the 1-vertex, 1-loop-edge graph.
 * https://oeis.org/A244880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (630 + 3051*n + 6570*n^2 + 8211*n^3 + 6503*n^4 + 3339*n^5
 *            * + 1085*n^6 + 204*n^7 + 17*n^8) / 630
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 400, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned long long coeffs[9] = {
        630ULL, 3051ULL, 6570ULL, 8211ULL, 6503ULL, 3339ULL, 1085ULL, 204ULL, 17ULL
    };
    unsigned __int128 power = 1;
    unsigned __int128 num   = 0;
    unsigned __int128 un    = n;
    for (int i = 0; i < 9; ++i) {
        num += (unsigned __int128)coeffs[i] * power;
        power *= un;
    }
    return (seq_t)(num / 630U);
}
