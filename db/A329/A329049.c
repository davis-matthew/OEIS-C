/*
 * A329049  Transpose of square array A329050.
 * https://oeis.org/A329049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = p_{j+1}^{2^i} where (i,j) are coordinates on the diagonal
 *            of array A329050 obtained by enumerating the array
 *            anti‑diagonally starting at a(0)=2. See OEIS A329049
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 19, u64 n <= 26, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow, get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow2.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d and the index i on that diagonal. */
    idx_t d = 0;            /* diagonal number (0 1 2 ...)           */
    idx_t cum = 0;          /* cumulative elements before this diag */

    while (1) {
        idx_t next_cum = cum + d + 1;   /* elements up to and including this diag */
        if (next_cum > n)
            break;
        cum = next_cum;
        ++d;
    }

    idx_t i = n - cum;     /* position within diagonal, row index 0…d   */
    idx_t j = d - i;       /* column index (prime index)              */

    seq_t base = (seq_t)get_prime(j + 1);  /* primes are 1‑based in get_prime  */
    seq_t exp  = pow2(i);                  /* 2^i                                 */
    return big_pow(base, exp);
}
