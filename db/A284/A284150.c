/*
 * A284150  Sum_{d|n, d==1 or 4 mod 5} d.
 * https://oeis.org/A284150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d ≡ 1 or 4 (mod 5)} d
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_divs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/sum_divs.h"

seq_t a(idx_t n)
{
    idx_t primes[16];
    idx_t exps[16];
    size_t count = 0;
    idx_t temp = n;

    for (idx_t i = 2; (unsigned __int128)i * i <= (unsigned __int128)temp; ++i) {
        if (temp % i == 0) {
            primes[count] = i;
            exps[count] = 1;
            temp /= i;
            while (temp % i == 0) {
                ++exps[count];
                temp /= i;
            }
            ++count;
        }
    }
    if (temp > 1) {
        primes[count] = temp;
        exps[count] = 1;
        ++count;
    }

    return sum_divs((seq_t)1, 0, primes, exps, count);
}
