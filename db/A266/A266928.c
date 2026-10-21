/*
 * A266928  Number of steps needed to reach 1 or 2 where a step is x -> 3x+1
 *          if x is odd, or x -> x/2p if x is even, 2p being the smallest
 *          semiprime dividing x.
 * https://oeis.org/A266928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = number of steps needed to reach 1 or 2 in
 * the variant Collatz sequence where a step is
 *
 *   if x is odd:   x = 3*x + 1
 *   if x is even:  x = x/(2*p)  where 2*p is the smallest semiprime
 *                      dividing x (i.e. 2*p with p the smallest prime factor
 *                      of x/2).                                            */
#include <stdint.h>

static inline uint64_t smallest_prime_factor_uint64(uint64_t n)
{
    if (n % 2ULL == 0ULL) return 2ULL;       /* smallest prime factor is 2 */
    if (n % 3ULL == 0ULL) return 3ULL;
    if (n % 5ULL == 0ULL) return 5ULL;
    uint64_t i = 7ULL;
    uint64_t step = 4ULL;                    /* 6k-1 , 6k+1 pattern */
    while (i <= n / i) {
        if (n % i == 0ULL) return i;
        i += step;
        step = 6ULL - step;                  /* toggle between 4 and 2 */
    }
    return n;                                /* n is prime */
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    seq_t steps = 0;

    while (x != 1ULL && x != 2ULL) {
        if ((x & 1ULL) == 0ULL) {              /* even */
            uint64_t y = x >> 1U;              /* y = x / 2 */
            uint64_t p = smallest_prime_factor_uint64(y);
            x = y / p;                         /* x = x / (2*p) */
        } else {                               /* odd */
            x = x * 3ULL + 1ULL;
        }
        ++steps;
    }
    return steps;
}
