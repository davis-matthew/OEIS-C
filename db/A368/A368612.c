/*
 * A368612  a(n) = Sum_{k=2..n} pi(k-1) * ceiling(n/k).
 * https://oeis.org/A368612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=2..n} pi(k-1) * ceil(n/k)
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   up_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/up_to.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    static uint8_t *prime_flag = NULL;
    static idx_t  *pi_arr     = NULL;
    static idx_t   max_limit  = 0;

    up_to(n, &prime_flag, &pi_arr, &max_limit);

    seq_t sum = 0;
    for (idx_t k = 2; k <= n; ++k) {
        seq_t pi_val  = (seq_t)pi_arr[k - 1];
        seq_t ceil_val = (seq_t)((n + k - 1) / k);  /* ceil(n/k) */
        sum += pi_val * ceil_val;
    }
    return sum;
}
