/*
 * A274503  a(n) = 301*binomial(n-1,8)+52*binomial(n-1,7)+binomial(n-1,6).
 * https://oeis.org/A274503
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 301*binomial(n-1,8) + 52*binomial(n-1,7) +
 *            binomial(n-1,6)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 33, u64 n <= 476,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 5  (a(5) is the first term) */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;                    /* undefined but safe for indices below offset */

    idx_t m = n - 1;                  /* use n-1 for binomial arguments */

    seq_t term8 = 0, term7 = 0, term6 = 0;

    if (m >= 8)
        term8 = binomial(m, 8);
    if (m >= 7)
        term7 = binomial(m, 7);
    if (m >= 6)
        term6 = binomial(m, 6);

    return term8 * (seq_t)301 + term7 * (seq_t)52 + term6;
}
