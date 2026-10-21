/*
 * A266667  Binary representation of the n-th iteration of the "Rule 51"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A266667: Binary representation of the n‑th iteration of Rule 51
   starting with a single ON cell.
   For even n, a(n) = 10^n.
   For odd n, a(n) is a decimal string of length 2n+1 consisting of an
   isolated 0 in the middle surrounded by 1’s (e.g. n=3 → 1110111).
   This implementation constructs the decimal value digit by digit,
   so that no intermediate value can overflow when the final result
   fits in seq_t.  */
seq_t a(idx_t n)
{
    seq_t res = 0;

    if ((n & 1) == 0) {                /* even n : 10^n */
        for (idx_t i = 0; i <= n; ++i) {
            res = res * (seq_t)10 + (i == 0 ? (seq_t)1 : (seq_t)0);
        }
    } else {                           /* odd n  : 1…1 0 1…1 (length 2n+1) */
        idx_t len = 2 * n + 1;
        for (idx_t i = 0; i < len; ++i) {
            res = res * (seq_t)10 + ((i == n) ? (seq_t)0 : (seq_t)1);
        }
    }

    return res;
}
