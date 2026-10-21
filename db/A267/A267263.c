/*
 * A267263  Number of nonzero digits in representation of n in primorial base.
 * https://oeis.org/A267263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of non-zero digits in the primorial base
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Count the non‑zero digits in the primorial base representation of n.
   The representation uses digits d_k with 0 ≤ d_k < prime_{k+1} and
   positions weighted by the product of the first k primes
   (P_0 = 1, P_{k+1} = P_k * prime_{k+1}).  The function iterates
   while the remaining value is > 0, dividing by successive primes and
   counting digits that are non‑zero. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t val = n;
    seq_t count = 0;
    idx_t month = 0;   /* index of the current digit (0‑based) */

    while (val > 0)
    {
        idx_t prime = get_prime(month + 1);   /* primes are 1‑based */
        idx_t digit = val % prime;
        if (digit != 0)
            count++;

        val = val / prime;
        month++;
    }
    return count;
}
