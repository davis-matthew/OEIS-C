/*
 * A367481  Primitive practical numbers of the form 2 * 3^i * prime(k).
 * https://oeis.org/A367481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 2 * 3^(floor(log_3(2*prime(n+2)))-1) * prime(n+2)
 * Range:     u8 n <= 4, u16 n <= 70, u32 n <= 8578, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* Compute a(n) for the OEIS sequence A367481.
 * The function works for the general seq_t and idx_t types.
 */
seq_t a(idx_t n)
{
    /* Step 1: Retrieve the (n+2)-th prime. */
    idx_t k = n + 2;
    uint32_t pk = get_prime(k);          /* get_prime returns a 32‑bit prime. */
    seq_t p = (seq_t)pk;                 /* cast to seq_t for further computation */

    /* Step 2: Compute m = 2 * p. */
    seq_t m = (seq_t)2 * p;

    /* Step 3: Determine e = floor(log_3(m)).  Find the largest
     * power of 3 that is <= m.  This gives e, and the desired
     * exponent for 3^i is i = e - 1.  Since m >= 10 for all n >= 1,
     * we have e >= 2 so the division below is safe. */
    seq_t power = 1;   /* 3^0 */
    idx_t e = 0;
    while (power * 3 <= m) {   /* power = 3^e */
        power *= 3;
        e++;
    }

    /* The factor 3^i where i = e - 1 is simply power / 3 */
    seq_t pow3 = power / 3;      /* 3^(e-1) */

    /* Step 4: Assemble the final term: 2 * 3^i * p */
    seq_t result = (seq_t)2 * pow3 * p;
    return result;
}
