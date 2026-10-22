/*
 * A397690  Number of different ways to form n by concatenation, using the
 *          digits of the numbers from 0 to n-1.
 * https://oeis.org/A397690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Π_{d∈digits(n)} falling_factorial(count_digit(d,0..n‑1),
 *            count_digit(d,n))
 * Range:     u8 n <= 86, u16 n <= 130, u32 n <= 1010, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where falling_factorial(m,k)=m*(m‑1)*…*(m‑k+1).  For any n<10 the result is 0. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Count how many times each digit 0..9 appears in the integers 0..n-1 */
    uint64_t pool[10];
    memset(pool, 0, sizeof(pool));

    for (idx_t i = 0; i < n; ++i) {
        idx_t x = i;
        if (x == 0) {
            pool[0] += 1;
            continue;
        }
        while (x > 0) {
            uint8_t d = (uint8_t)(x % 10);
            pool[d] += 1;
            x /= 10;
        }
    }

    /* Count how many times each digit 0..9 appears in n itself */
    uint64_t need[10];
    memset(need, 0, sizeof(need));
    if (n == 0) {
        need[0] = 1;  /* representation of 0 is a single '0' digit */
    } else {
        idx_t x = n;
        while (x > 0) {
            uint8_t d = (uint8_t)(x % 10);
            need[d] += 1;
            x /= 10;
        }
    }

    /* Compute the product over digits, using falling factorials. */
    seq_t result = 1;
    for (int d = 0; d < 10; ++d) {
        uint64_t k = need[d];
        if (k == 0) continue;
        uint64_t m = pool[d];
        if (m < k) return 0;  /* impossible to form the number */

        /* compute m*(m-1)*...*(m-k+1) */
        for (uint64_t t = 0; t < k; ++t) {
            result = result * (seq_t)(m - t);
        }
    }

    return result;
}
