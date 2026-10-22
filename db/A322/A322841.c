/*
 * A322841  Number of positive integers less than n with more distinct prime
 *          factors than n.
 * https://oeis.org/A322841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ m : 1 <= m < n and ω(m) > ω(n) }|
 * Range:     u8 n <= 342, u16 n <= 7762, u32 n <= 7762, u64 n <= 7762,
 *            u128 n <= 7762, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where ω(k) is the number of distinct prime factors of k */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    static uint8_t *omega = NULL;
    static idx_t omega_limit = 0;

    if (n > omega_limit) {
        /* Allocate new array large enough for [0..n] */
        omega = (uint8_t*)realloc(omega, (n + 1) * sizeof(uint8_t));
        for (idx_t i = 0; i <= n; ++i) omega[i] = 0;

        /* Sieve to compute the number of distinct prime factors for each number */
        for (idx_t i = 2; i <= n; ++i) {
            if (omega[i] == 0) {          /* i is prime */
                for (idx_t j = i; j <= n; j += i) {
                    ++omega[j];
                }
            }
        }
        omega_limit = n;
    }

    uint8_t omega_n = omega[n];
    seq_t count = 0;
    for (idx_t m = 1; m < n; ++m) {
        if (omega[m] > omega_n) {
            ++count;
        }
    }
    return count;
}
