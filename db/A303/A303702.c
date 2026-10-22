/*
 * A303702  Number of ways to write 2*n as p + 2^k + 3^m, where p is a prime,
 *          and k and m are nonnegative integers.
 * https://oeis.org/A303702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write 2*n as p + 2^k + 3^m, * where p
 *            is a prime, and k,m are nonnegative integers
 * Range:     u8 n <= 537, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1, a(1) defined below */

    seq_t count = 0;
    /* target = 2 * n, all arithmetic done in seq_t to avoid overflow */
    seq_t target = (seq_t)n * 2;
    if (target < 2) return 0;           /* 2*n must be at least 2 */

    seq_t min_rem = target - 2;         /* remaining after p >= 2 */

    seq_t pow2k = 1;                    /* current value of 2^k */
    while (pow2k <= min_rem) {
        seq_t max_pow3 = min_rem - pow2k;

        seq_t pow3m = 1;                /* current value of 3^m */
        while (pow3m <= max_pow3) {
            seq_t sum = pow2k + pow3m;
            seq_t p = target - sum;
            if (p >= 2 && is_prime_small(p)) {
                count++;
            }
            /* Prepare next power of 3, guard against overflow */
            if (pow3m > max_pow3 / 3) break;
            pow3m *= 3;
        }

        /* Prepare next power of 2, guard against overflow */
        if (pow2k > min_rem / 2) break;
        pow2k *= 2;
    }

    return count;
}
