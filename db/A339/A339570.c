/*
 * A339570  Denote the van der Corput sequence of fractions 1/2, 1/4, 3/4,
 *          1/8, 5/8, 3/8, 7/8, 1/16, ... (A030101/A062383) by v(n), n >= 1.
 *          Then a(n) = denominator of v(A014486(n)).
 * https://oeis.org/A339570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 4^k, where k is the smallest integer such that
 *            sum_{i=1}^k Catalan(i) >= n. (Catalan numbers are C_0=1, C_{i+1}
 *            = C_i * (4*i+2) / (i+2).)
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* pow2(idx_t exp) -> seq_t */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n=0, return 0 as guard */

    seq_t cum = 0;      /* cumulative sum of Catalan numbers up to current k */
    seq_t cat = 1;      /* start with C0 = 1 */

    for (idx_t i = 0; ; ++i) {
        /* compute C_{i+1} from C_i */
        cat = (cat * ((seq_t)4 * i + 2)) / (i + 2);

        idx_t k = i + 1;     /* current Catalan index */
        cum += cat;          /* cumulative sum up to k */

        if (cum >= n) {
            /* a(n) = 4^k = 2^{2k} */
            return pow2(2 * k);
        }
    }
}
