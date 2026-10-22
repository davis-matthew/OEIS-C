/*
 * A372717  Triangle read by rows: If k is prime then T(n, k) is the exponent
 *          of the highest power of k that divides n. T(0, 0) = T(1, 1) = 1.
 *          In all other cases T(n, k) = 0.
 * https://oeis.org/A372717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A372717:  Triangle read by rows
   a(n) = exponent of the prime k dividing r,
   where (r,k) is obtained by inverting the triangular enumeration:
   find the smallest r with r*(r+1)/2 > n, set k = n - r*(r+1)/2.
   Special cases: a(0)=1 (T(0,0)), a(2)=1 (T(1,1)); otherwise 0 for non‑prime k. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;          /* T(0,0) = 1 */

    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low = 0, high = n + 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        idx_t s = mid * (mid + 1) / 2;
        if (s <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;
    idx_t k = n - r * (r + 1) / 2;

    /* Special case: T(1,1) = 1 */
    if (r == (idx_t)1 && k == (idx_t)1)
        return (seq_t)1;

    /* Non‑prime or zero offsets yield 0. */
    if (k == (idx_t)0 || k == (idx_t)1)
        return (seq_t)0;
    if (!is_prime(k))
        return (seq_t)0;

    /* Compute exponent of prime k in r. */
    seq_t exp = (seq_t)0;
    idx_t tmp = r;
    while (tmp % k == (idx_t)0) {
        exp++;
        tmp /= k;
    }
    return exp;
}
