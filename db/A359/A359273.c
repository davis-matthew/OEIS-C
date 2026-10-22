/*
 * A359273  a(n) = least positive integer k such that (prime(n+k)-prime(n))/n
 *          is an integer.
 * https://oeis.org/A359273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer k such that
 *            (prime(n+k)-prime(n))/n is an integer
 * Range:     u8 n <= 40, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* behaviour for n<1 is irrelevant */

    uint32_t p_n = get_prime(n);          /* prime(n) */
    idx_t k;

    for (k = 1; ; ++k) {
        uint32_t p_m = get_prime(n + k);  /* prime(n+k) */
        seq_t diff = (seq_t)p_m - (seq_t)p_n;
        if (diff % n == 0) return (seq_t)k;
    }
}
