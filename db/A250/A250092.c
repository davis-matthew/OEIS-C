/*
 * A250092  a(n) = binomial( prime(n+4), prime(n) ).
 * https://oeis.org/A250092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(prime(n+4), prime(n))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is first term.  For safety guard n==0. */
    if (n == 0) return (seq_t)0;
    idx_t p_n   = get_prime(n);
    idx_t p_np4 = get_prime(n + 4);
    return binomial(p_np4, p_n);
}
