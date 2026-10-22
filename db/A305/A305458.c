/*
 * A305458  In primorial base: a(n) is obtained by replacing each nonzero
 *          digit of n with its product with the nonzero digits at lower
 *          indices (See Comments for precise definition).
 * https://oeis.org/A305458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n)=∑_{i≥0} e_i·P_i where P_i=∏_{k=1..i}prime(k) and for the
 *            primorial‑base digits d_j of n: e_i = 0 if d_i = 0 e_i =
 *            (∏_{j≤i,d_j>0} d_j) mod prime(i+1) otherwise (the product is
 *            taken over all non‑zero digits up to the i‑th position)
 * Range:     u8 n <= 213, u16 n <= 2310, u32 n <= 2310, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 0;      /* accumulated value */
    seq_t weight = 1;      /* primorial weight P_i  (product of first i primes) */
    seq_t product = 1;     /* full product of all non‑zero digits so far */

    idx_t rest = n;        /* remaining part of n in primorial base */
    idx_t i = 0;           /* digit index (0‑based) */

    for (;; ++i)
    {
        uint32_t prime = get_prime(i + 1);          /* prime(i+1) */
        uint64_t digit = rest % (uint64_t)prime;    /* current digit d_i */
        rest /= (uint64_t)prime;                    /* shift right for next digit */

        seq_t ei = 0;
        if (digit != 0)
        {
            product = product * (seq_t)digit;          /* exact product so far */
            ei      = product % (seq_t)prime;         /* product mod current prime */
        }

        result += ei * weight;                     /* add contribution */
        weight *= (seq_t)prime;                    /* advance weight to next primorial */

        if (rest == 0) break;                      /* finished all digits */
    }

    return result;
}
