/*
 * A370883  Irregular triangle read by rows: T(n,k) is the number of unmatched
 *          right parentheses in the k-th string of parentheses of length n,
 *          where strings within a row are in reverse lexicographical order.
 * https://oeis.org/A370883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by: for the binary string s of length r * where
 *            r = ⌊log2(n+1)⌋, let i = n - (2^r - 1). The i‑th string in *
 *            reverse lexicographic order corresponds to the integer * x =
 *            (2^r - 1) - i. Interpret x in binary with '(' for 0 and ')' for
 *            * 1. Count the number of unmatched right paren...
 * Range:     u8 n <= 8826, u16 n <= 8826, u32 n <= 8826, u64 n <= 8826,
 *            u128 n <= 8826, big any n
 * Helpers:   is_log2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_log2.h"

seq_t a(idx_t n) {
    /* Determine the row r and the position i within that row. */
    idx_t m = n + 1;                       /* m = n+1 >= 1 */
    idx_t r = is_log2(m);                  /* 2^r <= m < 2^(r+1) */
    idx_t shift = 1ULL << r;               /* 2^r */
    idx_t start = shift - 1;               /* starting index of row r */
    idx_t i = n - start;                   /* 0 <= i < 2^r */

    /* Compute the binary representation x of the string. */
    idx_t x = (shift - 1) - i;             /* value of the string */

    /* Count unmatched right parentheses. */
    seq_t unmatched = 0;
    seq_t balance = 0;

    for (idx_t pos = r; pos > 0; --pos) {
        bool bit = (x >> (pos - 1)) & 1ULL;   /* 1 -> ')' , 0 -> '(' */
        if (!bit) {                           /* '(' */
            ++balance;
        } else {                              /* ')' */
            if (balance > 0) {
                --balance;
            } else {
                ++unmatched;
            }
        }
    }

    return unmatched;
}
