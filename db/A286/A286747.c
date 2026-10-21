/*
 * A286747  Characteristic sequence of the Beatty sequence, A177102, of
 *          sqrt(10).
 * https://oeis.org/A286747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - floor((n+1)*(1-1/r)) + floor(n*(1-1/r)) where r =
 *            sqrt(10)
 * Range:     u8 n <= 6, u16 n <= 86, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; behaviour for 0 is irrelevant */

    seq_t n_s   = (seq_t)n;        /* cast index to seq_t */
    seq_t sq    = n_s * n_s;       /* n^2 */
    seq_t div10 = sq / 10;         /* integer floor(n^2/10)          */
    seq_t k     = floor_sqrt(div10) + 1;   /* floor(n/√10) + 1              */
    seq_t term  = floor_sqrt((seq_t)10 * k * k); /* floor(k*√10)                 */

    return (term == n_s) ? 1 : 0;  /* 1 if n is a Beatty term, else 0 */
}
