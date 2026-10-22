/*
 * A355035  Consider the least base b >= 2 where the sum of digits of n is a
 *          prime number; a(n) corresponds to this prime number.
 * https://oeis.org/A355035
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A216789(n, A355034(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

/* Return the prime digit-sum obtained from the smallest base b ≥ 2
   such that the sum of the digits of n in base b is prime. */
seq_t a(idx_t n)
{
    if (n < 2)            /* undefined for n < 2; safe fall‑back */
        return 0;

    /* Search bases from 2 up to n. */
    for (idx_t b = 2; b <= n; ++b) {
        idx_t  tmp = n;
        seq_t sum = 0;
        while (tmp) {
            sum += tmp % b;
            tmp /= b;
        }
        if (is_prime_small(sum))
            return sum;
    }

    /* If the loop finishes without finding a prime sum, consider base n+1,
       which yields the single digit representation n. */
    if (n != UINT64_MAX) {
        idx_t  tmp = n;
        seq_t sum = 0;
        while (tmp) {
            sum += tmp % (n + 1);
            tmp /= (n + 1);
        }
        if (is_prime_small(sum))
            return sum;
    }

    return 0;   /* should never happen for n ≥ 2 */
}
