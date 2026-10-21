/*
 * A248686  Triangular array of multinomial coefficients: T(n,k) =
 *          n!/(n(1)!*n(2)!* ... *n(k)!), where n(i) = floor((n + i - 1)/k)
 *          for i = 1 .. k.
 * https://oeis.org/A248686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where r = ceil((sqrt(8*n-1)-1)/2), c = n -
 *            r*(r-1)/2. T(r,c) = r! / Π_{i=1}^{c} ⌊(r + i - 1)/c⌋!
 * Range:     u8 n <= 19, u16 n <= 42, u32 n <= 90, u64 n <= 229,
 *            u128 n <= 625, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset is 1; safeguard */

    /* Find row r and column c of the n-th element in the triangular array */
    idx_t r = 1;
    idx_t cum = 1;
    while (n > cum) {
        ++r;
        cum += r;
    }
    idx_t c = n - (cum - r);

    /* Compute the multinomial coefficient via successive binomials */
    seq_t total = 1;
    idx_t remaining = r;
    for (idx_t i = 1; i <= c; ++i) {
        idx_t ni = (r + i - 1) / c;
        seq_t choose = binomial(remaining, ni);
        total = total * choose;
        remaining -= ni;
    }
    return total;
}
