/*
 * A319085  a(n) = Sum_{k=1..n} k^2*tau(k), where tau is A000005.
 * https://oeis.org/A319085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1176, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=1..n} k^2 * tau(k),  tau(k) = number of divisors of k
 *
 * The implementation builds the divisor function up to n using a sieve
 * and accumulates the sum in a seq_t value.  It works for any integer
 * width chosen for seq_t, including arbitrary‑precision big integers,
 * and is valid under both C11 and C++17.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* no term below the offset */

    /* Allocate temporary array for tau(k) values, all initialized to 0. */
    uint64_t *tau = (uint64_t*)calloc((size_t)(n + 1), sizeof(uint64_t));
    if (!tau) return 0;             /* allocation failure; return 0 */

    /* Sieve: count divisors for each number up to n. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            ++tau[j];
        }
    }

    /* Accumulate the sum k^2 * tau(k). */
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        kk = kk * kk;               /* k^2 */
        result += kk * (seq_t)tau[k];
    }

    free(tau);
    return result;
}
