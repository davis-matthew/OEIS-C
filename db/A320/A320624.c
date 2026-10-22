/*
 * A320624  a(n) = (A006134((prime(n)-1)/2) - Legendre(prime(n), 3))/prime(n).
 * https://oeis.org/A320624
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A006134((prime(n)-1)/2) - Legendre(prime(n),3)) /
 *            prime(n) where A006134(m) = sum_{i=0}^m binom(2i,i)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    if (n < 2)     /* offset is 2; behavior below is irrelevant */
        return (seq_t)0;

    /* get the nth prime (prime(1)=2, prime(2)=3, ...) */
    uint32_t p = get_prime(n);
    /* k = (p-1)/2 */
    idx_t k = (p - 1) / 2;

    /* compute S = sum_{i=0}^k binom(2*i,i)
       using unsigned __int128 to avoid overflow up to the first case
       where the result still fits in seq_t */
    unsigned __int128 sum = 0;
    unsigned __int128 binom = 1;   /* binom(0,0) = 1 */
    for (idx_t i = 0; i <= k; ++i) {
        if (i > 0)
            binom = (binom * (unsigned __int128)(4 * i - 2)) / (unsigned __int128)i;
        sum += binom;
    }

    /* Legendre symbol (p|3) via Jacobi symbol */
    int leg = jacobi_symbol(p, 3);

    /* numerator = S - leg */
    signed __int128 num = (signed __int128)sum - leg;
    signed __int128 div = num / (signed __int128)p;

    return (seq_t)div;
}
