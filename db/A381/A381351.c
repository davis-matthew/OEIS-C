/*
 * A381351  Number of subsets of 9 integers between 1 and n such that their
 *          sum is 3 modulo n.
 * https://oeis.org/A381351
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n ≥ 10, * if n mod 3 ≠ 0 : a(n) = C(n,9) / n * if n mod 3 =
 *            0 but n mod 9 ≠ 0 : a(n) = (C(n,9) + 2·C(n/3,3)) / n * if n mod
 *            9 = 0 : a(n) = (C(n,9) + 2·C(n/3,3) – 3·(n/9)) / n
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   _binom3, _binom9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_binom3.h"
#include "../utils/_binom9.h"

seq_t a(idx_t n) {
    if (n < 10) return 0;   /* below the OEIS offset */
    seq_t res;
    seq_t bin9 = _binom9(n);
    if (n % 3 != 0) {
        res = bin9 / (seq_t)n;
    } else {
        seq_t bin3 = _binom3(n / 3);
        if (n % 9 != 0) {
            res = (bin9 + 2 * bin3) / (seq_t)n;
        } else {
            seq_t term = bin9 + 2 * bin3 - 3 * (seq_t)(n / 9);
            res = term / (seq_t)n;
        }
    }
    return res;
}
