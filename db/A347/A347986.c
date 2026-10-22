/*
 * A347986  Length of identical sequence of prime / nonprime numbers left and
 *          right of the integer n (excluded).
 * https://oeis.org/A347986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{k ≥ 0 | for all j=1..k, is_prime(n-j) ==
 *            is_prime(n+j)}
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* external linkage */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */
    idx_t j;
    for (j = 1; ; ++j) {
        idx_t left = n - j;
        if (left < 1) break;              /* left bound exceeded */
        idx_t right = n + j;
        if (is_prime(left) != is_prime(right)) break;
    }
    return (seq_t)(j - 1);
}
