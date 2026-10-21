/*
 * A267535  Binary representation of the n-th iteration of the "Rule 143"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (OEIS style):
    a(0) = 1
    a(1) = 110
    a(n) = 1100*10^(2*n-3) + (10^(2*n-3)-1)/9,   for n ≥ 2
    This is the decimal value of the binary string “1100” followed by (2n‑3) ones.
*/

seq_t a(idx_t n)
{
    /* handle the trivial cases */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        seq_t r = 0;
        r = r * (seq_t)10 + (seq_t)1;
        r = r * (seq_t)10 + (seq_t)1;
        r = r * (seq_t)10 + (seq_t)0;
        return r;
    }

    /* n ≥ 2: start with the digits 1,1,0,0 */
    seq_t res = 0;
    res = res * (seq_t)10 + (seq_t)1;
    res = res * (seq_t)10 + (seq_t)1;
    res = res * (seq_t)10 + (seq_t)0;
    res = res * (seq_t)10 + (seq_t)0;

    /* append (2*n-3) ones */
    idx_t ones = 2 * n - 3;
    for (idx_t i = 0; i < ones; ++i) {
        res = res * (seq_t)10 + (seq_t)1;
    }

    return res;
}
