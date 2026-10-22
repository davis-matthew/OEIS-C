/*
 * A302033  a(n) = A019565(A003188(n)).
 * https://oeis.org/A302033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 81, u32 n <= 681, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A019565(A003188(n))
 *   a(0) = 1
 *   For n >= 0, let e = v2(n+1) (exponent of 2 in n+1).
 *   Let p = prime_{e+1}.
 *   If a(n) divisible by p, a(n+1) = a(n) / p
 *   else a(n+1) = a(n) * p
 *
 * This implements the sequence in O(n) time with small integer
 * arithmetic that works for any seq_t type supporting +, -, *, /,
 * and %.
 */

#include <stdint.h>

/* first few primes – enough for all practical n (e up to ~20) */
static const uint32_t _primes[30] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113
};

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;          /* a(0) = 1 */
    if (n == 0)
        return val;

    for (idx_t j = 1; j <= n; ++j)   /* j = n+1 in the recurrence */
    {
        /* exponent of 2 in j */
        idx_t tmp = j;
        int e = 0;
        while ((tmp & 1ULL) == 0) {
            ++e;
            tmp >>= 1;
        }

        /* p = prime_{e+1} */
        uint32_t p = _primes[e];    /* e is zero‑based index */

        /* decide multiply or divide */
        if ((val % (seq_t)p) == 0)
            val /= (seq_t)p;        /* divide by p */
        else
            val *= (seq_t)p;        /* multiply by p */
    }
    return val;
}
