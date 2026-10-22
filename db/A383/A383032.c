/*
 * A383032  Exponent of the highest power of 2 dividing the n-th number that
 *          is either a square or twice a square.
 * https://oeis.org/A383032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 779, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383032:
   a(n) = exponent of the highest power of 2 dividing
   the n‑th integer that is a square or twice a square. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) {                 /* undefined for n = 0, but return 0 */
        return 0;
    }

    idx_t i = 1;                  /* current index for squares    */
    idx_t j = 1;                  /* current index for twice‑squares */
    seq_t number = 0;             /* holds the n‑th number */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t vi = (seq_t)i * (seq_t)i;
        seq_t vj = ((seq_t)2) * (seq_t)j * (seq_t)j;
        if (vi < vj) {
            number = vi;
            ++i;
        } else {
            number = vj;
            ++j;
        }
    }

    /* compute v2(number) */
    seq_t exp = 0;
    seq_t tmp = number;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        ++exp;
    }
    return exp;
}
