/*
 * A333644  a(n) = Sum_{k=1..n} floor(n/k) * prime(k).
 * https://oeis.org/A333644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} ⌊n/k⌋ * prime(k) (OEIS A333644)
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    if (n == 0)               /* below offset – behaviour irrelevant */
        return (seq_t)0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t f = n / k;                 /* ⌊n/k⌋ */
        uint32_t pk = get_prime(k);      /* prime(k) */
        sum += (seq_t)f * (seq_t)pk;     /* accumulate in seq_t */
    }
    return sum;
}
