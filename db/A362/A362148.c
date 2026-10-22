/*
 * A362148  Numbers that are neither cubefree nor cubefull.
 * https://oeis.org/A362148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*------------------------------------------------------------
 * Formula: a(n) is the nth positive integer that is neither
 * cubefree nor cubefull (OEIS A362148).  The function finds the
 * term by scanning integers, testing the definition.
 *------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>

/* True if x has a prime exponent ≥ 3 and has at least one prime
   exponent < 3, i.e. it is neither cubefree nor cubefull. */
static bool in_sequence(uint64_t x)
{
    bool has_cube  = false;   /* at least one exponent ≥ 3   */
    bool has_small = false;   /* at least one exponent 1 or 2*/

    uint64_t n = x;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            int e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            if (e >= 3)
                has_cube = true;
            else
                has_small = true;
            if (has_cube && has_small)
                return true;
        }
    }

    /* remaining prime factor with exponent 1 */
    if (n > 1)
    {
        if (has_cube)
            has_small = true;
    }

    return has_cube && has_small;
}

/*------------------------------------------------------------
 * Public function: returns the nth term of A362148
 *------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* safety; offset is 1 */

    idx_t   count = 0;
    uint64_t candidate = 1;

    while (1)
    {
        ++candidate;
        if (in_sequence(candidate))
        {
            ++count;
            if ((idx_t)count == n)
                return (seq_t)candidate;
        }
    }
}
