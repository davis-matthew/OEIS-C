/*
 * A378371  Distance between n and the least non prime power >= n, allowing 1.
 * https://oeis.org/A378371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least non‑prime‑power ≥ n − n Computation: iterate
 *            forward until a non‑prime‑power is found, testing for prime
 *            powers by trial division
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime_power(uint64_t m)
{
    if (m <= 1)
        return false;                    /* 1 is not a prime power */

    /* extract the first prime factor */
    uint64_t p = 0;
    if ((m & 1) == 0) {                   /* handle factor 2 */
        p = 2;
        do { m /= 2; } while ((m & 1) == 0);
    } else {
        for (uint64_t i = 3; i * i <= m; i += 2) {
            if (m % i == 0) {
                p = i;
                do { m /= i; } while (m % i == 0);
                break;
            }
        }
    }

    if (p == 0)            /* m itself is prime */
        return true;

    return m == 1;         /* only one distinct prime factor → prime power */
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* behaviour below the offset does not matter */

    uint64_t nn = (uint64_t)n;
    uint64_t m  = nn;

    for (;;) {
        if (!is_prime_power(m)) {
            seq_t diff = (seq_t)m - (seq_t)nn;
            return diff;
        }
        ++m;
    }
}
