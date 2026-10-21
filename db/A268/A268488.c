/*
 * A268488  Least number k of the form k = n*(k % 10) + [k / 10], where k % 10
 *          = last digit of k, [k / 10] = k without its last digit.
 * https://oeis.org/A268488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = if n mod 9 == 1 then (n-1)/9*10+1 * else if n mod 3 == 1
 *            then (n-1)/3*10+3 * else n*10-1 * (valid for n ≥ 1)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* value for offset‑0, unused here */

    if (n % 9ULL == 1ULL) {
        /* ((n-1)/9)*10 + 1 */
        return (seq_t)((n - 1) / 9ULL) * (seq_t)10 + (seq_t)1;
    } else if (n % 3ULL == 1ULL) {
        /* ((n-1)/3)*10 + 3 */
        return (seq_t)((n - 1) / 3ULL) * (seq_t)10 + (seq_t)3;
    } else {
        /* n*10 - 1 */
        return (seq_t)n * (seq_t)10 - (seq_t)1;
    }
}
