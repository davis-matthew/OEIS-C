/*
 * A369739  a(n) = Sum_{p|n, p prime} p^tau(n/p).
 * https://oeis.org/A369739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} p^tau(n/p) (a(1)=0)
 * Range:     u8 n <= 27, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   ipow, num_divisors, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow.h"
#include "../utils/num_divisors.h"
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* a(1) is defined as 0 */
    if (n <= 1)
        return 0;

    uint64_t orig_n = (uint64_t)n;   /* keep original n for n/p */
    uint64_t nn = orig_n;
    seq_t sum = 0;
    uint64_t last_p = 0;               /* last distinct prime processed */

    while (nn > 1)
    {
        uint64_t p = smallest_prime_factor(nn);
        if (p != last_p)
        {
            uint64_t m = orig_n / p;                    /* n / p */
            uint64_t tau_m = num_divisors(m);       /* tau(n/p) */
            seq_t term = ipow((seq_t)p, (seq_t)tau_m);/* p^tau(n/p) */
            sum += term;
            last_p = p;
        }

        /* Skip all copies of p to reach next distinct prime */
        while (nn % p == 0)
            nn /= p;
    }

    return sum;
}
