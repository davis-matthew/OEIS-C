/*
 * A306958  If the decimal expansion of n is d_1 ... d_k, a(n) = Sum
 *          d_i!*binomial(10,d_i).
 * https://oeis.org/A306958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = Sum_{i=1..k} d_i! * binomial(10,d_i) where d_i are the
 *            decimal digits of n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed contributions for each decimal digit
       d   0   1   2    3     4      5       6        7        8         9
       contrib = d! * C(10,d) = [1, 10, 90, 720, 5040, 30240, 151200, 604800, 1814400, 3628800] */
    static const seq_t contrib[10] = {
        1ULL,      // d = 0
        10ULL,     // d = 1
        90ULL,     // d = 2
        720ULL,    // d = 3
        5040ULL,   // d = 4
        30240ULL,  // d = 5
        151200ULL, // d = 6
        604800ULL, // d = 7
        1814400ULL,// d = 8
        3628800ULL // d = 9
    };

    seq_t result = 0;

    if (n == 0)             /* n = 0 has a single digit 0 */
        return contrib[0];

    while (n > 0) {
        idx_t digit = n % 10;   /* least significant decimal digit */
        result += contrib[digit];
        n /= 10;
    }

    return result;
}
