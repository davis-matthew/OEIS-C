/*
 * A266979  Binary representation of the n-th iteration of the "Rule 79"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = • if n is even: decimal string 1010… (length n+1,
 *            starting with 1) • if n is odd : first n+1 digits are all 1
 *            followed by n digits alternating 0,1 starting with 0 (total
 *            length 2n+1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    if ((n & 1u) == 0) {          /* even n: 1010... */
        idx_t len = n + 1;
        for (idx_t i = 0; i < len; ++i) {
            result = result * 10 + ((i & 1u) == 0 ? (seq_t)1 : (seq_t)0);
        }
    } else {                     /* odd n: (n+1)×1 followed by n×0101... */
        idx_t len = 2 * n + 1;
        for (idx_t i = 0; i < len; ++i) {
            seq_t digit;
            if (i <= n)          /* first n+1 digits are 1 */
                digit = 1;
            else {                /* alternating 0,1, starting with 0 */
                idx_t idx = i - (n + 1);
                digit = (idx & 1u) == 0 ? 0 : 1;
            }
            result = result * 10 + digit;
        }
    }
    return result;
}
