/*
 * A303399  Number of ordered pairs (a, b) with 0 <= a <= b such that n - 5^a
 *          - 5^b can be written as the sum of two triangular numbers.
 * https://oeis.org/A303399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (a,b) with 0 ≤ a ≤ b such that *
 *            n – 5^a – 5^b can be expressed as the sum of two triangular
 *            numbers
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_of_two_triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/sum_of_two_triangular.h"

seq_t a(idx_t n)
{
    /* Pre-compute powers of 5 once */
    static uint64_t pow5[65];
    static int pow5cnt;
    static bool initialized = false;
    if (!initialized)
    {
        pow5[0] = 1;
        pow5cnt = 1;
        while (pow5cnt < 65)
        {
            uint64_t next = pow5[pow5cnt - 1] * 5ULL;
            if (next / 5ULL != pow5[pow5cnt - 1])  /* overflow check */
                break;
            pow5[pow5cnt] = next;
            pow5cnt++;
        }
        initialized = true;
    }

    seq_t count = 0;

    /* Iterate over all pairs (a, b) with 0 ≤ a ≤ b */
    for (int a = 0; a < pow5cnt && pow5[a] <= n; a++)
    {
        for (int b = a; b < pow5cnt; b++)
        {
            seq_t sum = (seq_t)pow5[a] + (seq_t)pow5[b];
            if (sum > n)
                break;  /* further b will only increase the sum */
            seq_t m = (seq_t)n - sum;
            if (sum_of_two_triangular(m))
                count++;
        }
    }

    return count;
}
