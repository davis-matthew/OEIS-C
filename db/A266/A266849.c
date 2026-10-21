/*
 * A266849  Binary representation of the n-th iteration of the "Rule 71"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for even n: a(n) = 10^n for odd n: a(n) = '1' repeated (n+1)/2 +
 *            1 times, followed by a single '0', followed by '1' repeated
 *            (n-1)/2 times (which matches the first several terms: 1, 110,
 *            100, 1111011, …) The function below generates the decimal value
 *            of a(n) directly by repeated multiplication by...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    /* if n is even: 1 followed by n zeros */
    if ((n & 1) == 0) {
        val = 1;
        for (idx_t i = 0; i < n; ++i) {
            val *= 10;
        }
    } else { /* n is odd */
        /* first part: (n+1) ones */
        for (idx_t i = 0; i <= n; ++i) {  /* run n+1 times */
            val = val * 10 + 1;
        }
        /* zero digit */
        val *= 10;
        /* second part: (n-1) ones */
        if (n > 1) {
            for (idx_t i = 0; i < n - 1; ++i) {
                val = val * 10 + 1;
            }
        }
    }
    return val;
}
