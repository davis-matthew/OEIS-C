/*
 * A302553  Hyper-4 powers that are not hyper-5 powers.
 * https://oeis.org/A302553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n==1) ? 16 : (n+1)^(n+1) ( hyper‑4 powers that are not
 *            hyper‑5 powers )
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; behaviour for n<1 is irrelevant */
    if (n == 1) return (seq_t)16; /* 2^4 */
    idx_t m = n + 1;              /* now m = 3,4,5,... for n=2,3,4,... */
    return pow_int((seq_t)m, m);  /* compute m^m exactly in seq_t */
}
