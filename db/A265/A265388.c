/*
 * A265388  a(n) = gcd{k=1..n-1} binomial(2*n, 2*k), a(1) = 0.
 * https://oeis.org/A265388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcdₖ=1…n−1 binomial(2*n, 2*k)
 * Range:     u8 n <= 14, u16 n <= 198, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"           /* build_primes_upto */
#include "../utils/is_power_of_two.h"            /* is_power_of_two */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    static uint64_t *prime_arr = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    idx_t limit = 2 * n - 1;                     /* max prime needed */
    if (prime_arr == NULL || prime_limit < limit) {
        uint64_t *new_arr = NULL;
        size_t new_cnt = 0;
        build_primes_upto(limit, &new_arr, &new_cnt);
        if (prime_arr)
            free(prime_arr);
        prime_arr = new_arr;
        prime_cnt = new_cnt;
        prime_limit = limit;
    }

    seq_t result = (seq_t)1;
    if (n > 1 && is_power_of_two((seq_t)n))
        result *= (seq_t)2;                     /* 2‑factor */

    seq_t two_n = (seq_t)2 * n;
    seq_t pwrs[64];                              /* enough for any prime < 20000 */

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = prime_arr[pi];
        if ((idx_t)p == 2)                       /* skip 2 – handled above */
            continue;

        /* generate powers of p up to limit */
        size_t pc = 0;
        seq_t cur = (seq_t)1;
        while (cur <= (seq_t)limit) {
            pwrs[pc++] = cur;
            if (cur > (seq_t)limit / p)
                break;
            cur *= (seq_t)p;
        }

        /* check for i <= j with pwri + powj == 2n */
        bool found = false;
        for (size_t i = 0; !found && i < pc; ++i) {
            for (size_t j = i; j < pc; ++j) {
                if (pwrs[i] + pwrs[j] == two_n) {
                    found = true;
                    break;
                }
            }
        }

        if (found)
            result *= (seq_t)p;                  /* include p once */
    }

    return result;
}
