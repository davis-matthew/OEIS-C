/*
 * A276032  Number of refinements of the partition n^1 with all numbers taken
 *          modulo 2.
 * https://oeis.org/A276032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=1}^{n/2 + 1} Catalan(j) - ((n mod 2 == 0) ? 1 : 0)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1U)
        return 0;

    /* Number of Catalan terms needed for this n */
    idx_t m = n / 2 + 1;

    seq_t sum = 0;
    seq_t cat = 0;   /* Will hold C_j while iterating */

    if (m >= 1) {
        cat = 1;          /* C_1 */
        sum = 1;          /* add C_1 */

        for (idx_t j = 2; j <= m; ++j) {
            /* C_j = C_{j-1} * (4*j-2) / (j+1)  */
            seq_t num = (seq_t)(4 * j - 2);
            cat = (cat * num) / (seq_t)(j + 1);
            sum += cat;
        }
    }

    /* Even indices are one less than the following odd index */
    if ((n & 1U) == 0)
        sum -= 1;

    return sum;
}
