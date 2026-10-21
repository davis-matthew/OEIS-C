/*
 * A252308  Number of (3+2)X(n+2) 0..3 arrays with every 3X3 subblock row and
 *          diagonal sum equal to 0 1 3 6 or 7 and every 3X3 column and
 *          antidiagonal sum not equal to 0 1 3 6 or 7.
 * https://oeis.org/A252308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-3) - 14*a(n-6) + 8*a(n-9) for n >= 16 Base values
 *            (n = 1..15): 1303, 1168, 1677, 2287, 3902, 6103, 8650, 14935,
 *            23660, 33855, 58675, 93344, 134047, 232723, 370928
 * Range:     u8 none, u16 n <= 11, u32 n <= 35, u64 n <= 83, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base[0] unused to keep 1‑based indexing */
    static const seq_t base[16] = {
        0ULL,     /* base[0] placeholder */
        1303ULL, 1168ULL, 1677ULL, 2287ULL, 3902ULL,
        6103ULL, 8650ULL, 14935ULL, 23660ULL, 33855ULL,
        58675ULL, 93344ULL, 134047ULL, 232723ULL, 370928ULL
    };

    if (n <= 15) {
        return base[n];
    }

    /* circular buffer of 10 slots: indices modulo 10 */
    seq_t buf[10] = {0};
    idx_t i;

    /* initialise buffer with the first 15 terms */
    for (i = 1; i <= 15; ++i) {
        buf[i % 10] = base[i];
    }

    seq_t val;
    for (i = 16; i <= n; ++i) {
        val = 7 * buf[(i - 3) % 10];
        val -= 14 * buf[(i - 6) % 10];
        val += 8 * buf[(i - 9) % 10];
        buf[i % 10] = val;
    }

    return buf[n % 10];
}
