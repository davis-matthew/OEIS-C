/*
 * A353442  Integers m such that the decimal expansion of 1/m contains the
 *          digit 6.
 * https://oeis.org/A353442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sequential search for integers m such that the decimal expansion of 1/m contains the digit 6. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: return 1 if 1/m has a decimal digit 6, otherwise 0. */
static int contains6(seq_t m)
{
    if (m <= (seq_t)1)                /* 1/1 = 1.0 – no 6, and 1/0 is undefined */
        return 0;

    seq_t r = 1 % m;                  /* current remainder */
    /* The period of the decimal expansion is at most m-1 when gcd(m,10)=1.
       A simple bound of m steps prevents an infinite loop. */
    for (idx_t i = 0; i < (idx_t)m; ++i)
    {
        r *= (seq_t)10;               /* bring down next digit */
        seq_t digit = r / m;
        if (digit == (seq_t)6)        /* found a 6 */
            return 1;
        r %= m;                       /* next remainder */
        if (r == (seq_t)0)            /* terminating decimal */
            break;
    }
    return 0;
}

/* Main sequence function: a(n) returns the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined for n < 1 */
        return 0;

    idx_t count = 0;
    seq_t m = 1;
    while (true)
    {
        ++m;                          /* next integer to test */
        if (contains6(m))
        {
            ++count;
            if (count == n)
                return m;
        }
    }
}
