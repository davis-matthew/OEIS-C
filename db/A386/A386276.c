/*
 * A386276  Numbers k such that the sequence defined by f(1) = k and f(x+1) =
 *          the sum of the three largest proper divisors of f(x), consists
 *          entirely of numbers having at least three proper divisors.
 * https://oeis.org/A386276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * m(n) where m(n) is the nth positive integer whose
 *            prime factorization satisfies: * after removing all factors of
 *            12, the remaining part is odd * and not divisible by 5.
 *            Equivalently: m = 6 * i * 12^j with gcd(i,10)=1, j≥0
 * Range:     u8 n <= 19, u16 n <= 3709, u32 n <= 3709, u64 n <= 3709,
 *            u128 n <= 3709, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int is_valid(idx_t n)
{
    /* Strip all factors of 12. */
    while ((n % 12ULL) == 0ULL)
        n /= 12ULL;
    /* Remaining part must be odd and not divisible by 5. */
    return ((n & 1ULL) != 0ULL) && (n % 5ULL != 0ULL);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined, but guard against bad input */

    idx_t   count = 0;   /* how many valid numbers seen so far   */
    idx_t   candidate = 1; /* candidate integer m for the sequence */
    seq_t   result = 0;

    while (count < n)
    {
        if (is_valid(candidate))
        {
            result = (seq_t)6 * (seq_t)candidate;
            ++count;
        }
        ++candidate;
    }
    return result;
}
