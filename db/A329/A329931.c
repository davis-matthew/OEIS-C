/*
 * A329931  Reversal of base-n digits of largest prime < n^3.
 * https://oeis.org/A329931
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = reverse_base_n( prev_prime(n^3) , n )
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 44 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility to look up previous prime < x.  The compile environment
   supplies a header providing this function. */
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

/* Reverse the digits of ``num`` written in base ``base``.
   The result is interpreted in the same base. */
static seq_t reverse_base_n(idx_t num, idx_t base)
{
    seq_t rev = 0;
    idx_t digit;

    while (num > 0)
    {
        digit = num % base;           /* low‑order digit in base */
        rev = rev * (seq_t)base + (seq_t)digit;  /* prepend digit */
        num /= base;
    }
    return rev;
}

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
