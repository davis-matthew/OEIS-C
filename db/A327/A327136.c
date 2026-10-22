/*
 * A327136  Numbers k such that sin(2k) > sin(2k+2) < sin(2k+4).
 * https://oeis.org/A327136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth integer k such that sin(2k) > sin(2k+2) < sin(2k+4)
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The inequality is equivalent to 2k+1 mod 2π belonging to (3π/2−2 , 3π/2). */

#include <stdint.h>
#include <stdbool.h>

static const long double PI = 3.141592653589793238462643383279502884L;
static const uint64_t SCALE = 1000000000ULL;          /* scaling factor for precision */

static const uint64_t TWOPI_SCALED = (uint64_t)((2.0L * PI * SCALE) + 0.5L);
static const uint64_t LOW   = (uint64_t)(((3.0L * PI) / 2.0L - 2.0L) * SCALE + 0.5L);
static const uint64_t HIGH  = (uint64_t)((3.0L * PI / 2.0L) * SCALE + 0.5L);

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for offset 1, return 0 */

    idx_t target = n;
    idx_t count  = 0;

    for (idx_t k = 1; ; ++k)                    /* start from k = 1 (a(1)=1) */
    {
        uint64_t kk    = (uint64_t)k;
        __uint128_t prod = (__uint128_t)((uint64_t)((2 * kk) + 1)) * SCALE;
        uint64_t t_scaled = (uint64_t)(prod % TWOPI_SCALED);

        if (t_scaled >= LOW && t_scaled < HIGH)
        {
            ++count;
            if (count == target)
                return (seq_t)k;
        }
    }
}
