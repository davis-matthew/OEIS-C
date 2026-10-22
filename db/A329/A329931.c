/*
 * A329931  Reversal of base-n digits of largest prime < n^3.
 * https://oeis.org/A329931
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = reverse_base_n( prev_prime(n^3) , n )
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   prev_prime, reverse_base_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility to look up previous prime < x.  The compile environment
   supplies a header providing this function. */
#include "../utils/prev_prime.h"
#include "../utils/reverse_base_n.h"

seq_t a(idx_t n)
{
    if (n < 2)                      /* sequence defined for n ≥ 2 */
        return 0;

    /* Compute n³.  For n up to ~2·10⁶ the product fits in idx_t. */
    idx_t cube = n * n * n;

    /* Largest prime strictly below n³. */
    idx_t p = prev_prime(cube);

    /* Reverse base‑n digits of that prime. */
    return reverse_base_n(p, n);
}
