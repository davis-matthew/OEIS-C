/*
 * A395632  a(n) = number of triples (x, y, z) such that 2*x^2 + y*z = n,
 *          where x, y, z are positive integers satisfying x < y <= z.
 * https://oeis.org/A395632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of triples (x,y,z) with x<y≤z and 2*x^2 + y*z = n.
   The function returns a(idx_t n) as defined in the OEIS entry. */
#include <stdint.h>

static idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0, bit = ((idx_t)1ULL) << 62;   /* highest power of four ≤ n */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 1; ; ++x) {
        idx_t two_x = 2 * x;
        /* If 2*x^2 will exceed n, stop.  Checking via division avoids overflow. */
        if (two_x > n / x) break;
        idx_t two_x2 = two_x * x;    /* = 2*x^2, guaranteed ≤ n */
        idx_t rem = n - two_x2;      /* remaining value = y*z */

        if (rem == 0) continue;      /* cannot have y or z = 0 */

        idx_t sqrt_rem = isqrt_u64(rem);
        /* y must be > x and ≤ sqrt(rem) */
        for (idx_t y = x + 1; y <= sqrt_rem; ++y) {
            if (rem % y == 0) {
                idx_t z = rem / y;
                if (z >= y) ++count; /* x < y ≤ z satisfied */
            }
        }
    }
    return count;
}
