/*
 * A334207  Number of ways to write 2n as the sum of two nonprime positive
 *          integers.
 * https://oeis.org/A334207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (1 - c(i)) * (1 - c(2*n - i))
 * Range:     u8 n <= 344, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static bool *prime_arr = NULL;
static idx_t _prime_limit = 0;

static void _ensure_prime_sieve(idx_t limit)
{
    if (limit <= _prime_limit)
        return;
    if (prime_arr)
        free(prime_arr);
    prime_arr = (bool *)malloc((size_t)(limit + 1));
    if (!prime_arr)
        abort();
    for (idx_t i = 0; i <= limit; ++i)
        prime_arr[i] = true;
    if (limit >= 0)
        prime_arr[0] = false;
    if (limit >= 1)
        prime_arr[1] = false;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (prime_arr[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                prime_arr[m] = false;
        }
    }
    _prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset starts at 1; return 0 for out of range */
    idx_t two_n = n << 1;         /* 2*n */
    _ensure_prime_sieve(two_n);
    seq_t cnt = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t j = two_n - i;
        if (!prime_arr[i] && !prime_arr[j])
            ++cnt;
    }
    return cnt;
}
