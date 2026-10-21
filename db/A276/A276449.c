/*
 * A276449  Number of 1-orbits of the cyclic group C_4 for a bi-colored square
 *          n X n grid with n squares of one color.
 * https://oeis.org/A276449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial((2*i)^2,i) for n = 4*i, a(n) =
 *            binomial((2*i)*(2*i+1),i) for n = 4*i+1, a(n) = 0 otherwise
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 23, u64 n <= 39, u128 n <= 67,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The helper binomial() is provided elsewhere, e.g.
    #include "../utils/binomial.h"  */

#include "../utils/binomial.h"

/* seq_t and idx_t are defined by the build. */
seq_t a(idx_t n) {
    if (n == 0) {          /* offset starts at 1, but guard anyway */
        return 0;
    }

    idx_t mod = n % 4;

    if (mod == 0) {        /* n = 4*i */
        idx_t i = n / 4;
        return binomial((2*i)*(2*i), i);
    }
    if (mod == 1) {        /* n = 4*i + 1 */
        idx_t i = (n - 1) / 4;
        return binomial((2*i)*(2*i + 1), i);
    }

    return 0;              /* other residues give zero */
}
