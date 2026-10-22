/*
 * A395852  Divisors of the order of the Tits group, |T| = 17971200.
 * https://oeis.org/A395852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 226, u32 n <= 288, u64 n <= 288,
 *            u128 n <= 288, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 17971200 sorted ascending.
   a(n) = the n-th smallest divisor of 17971200 (1 ≤ n ≤ 288). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define DIV_COUNT 288   /* (11+1)(3+1)(2+1)(1+1) */

static uint64_t divisors[DIV_COUNT];
static bool initialized = false;

static void init_divisors(void)
{
    if (initialized)
        return;

    /* Pre‑compute powers of the prime factors. */
    uint64_t pow2[12], pow3[4], pow5[3], pow13[2];
    pow2[0] = 1;
    for (size_t i = 1; i < 12; ++i)
        pow2[i] = pow2[i - 1] * 2ULL;
    pow3[0] = 1;
    for (size_t i = 1; i < 4; ++i)
        pow3[i] = pow3[i - 1] * 3ULL;
    pow5[0] = 1;
    for (size_t i = 1; i < 3; ++i)
        pow5[i] = pow5[i - 1] * 5ULL;
    pow13[0] = 1;
    pow13[1] = 13ULL;

    /* Generate all 2^3^10^2*13 types of divisors. */
    size_t idx = 0;
    for (size_t e2 = 0; e2 <= 11; ++e2)
        for (size_t e3 = 0; e3 <= 3; ++e3)
            for (size_t e5 = 0; e5 <= 2; ++e5)
                for (size_t e13 = 0; e13 <= 1; ++e13)
                {
                    divisors[idx++] = pow2[e2] * pow3[e3] * pow5[e5] * pow13[e13];
                }

    /* Sort the divisors ascending. */
    for (size_t i = 1; i < DIV_COUNT; ++i)
    {
        uint64_t key = divisors[i];
        size_t j = i;
        while (j > 0 && divisors[j - 1] > key)
        {
            divisors[j] = divisors[j - 1];
            --j;
        }
        divisors[j] = key;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init_divisors();

    if (n < 1 || (size_t)(n - 1) >= DIV_COUNT)
        return 0;           /* undefined – return 0 safely */

    return (seq_t)divisors[(size_t)(n - 1)];
}
