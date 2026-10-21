/*
 * A278907  a(n) = floor((n*2^(n+1)+2)/(2*n-(-1)^n+3)) -
 *          floor((n*2^(n+1)-2)/(2*n-(-1)^n+3)).
 * https://oeis.org/A278907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = floor((n*2^(n+1)+2)/(2*n-(-1)^n+3)) -
 *            floor((n*2^(n+1)-2)/(2*n-(-1)^n+3))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for the OEIS sequence A278907. */
seq_t
a(idx_t n)
{
    /* Special case for n = 0: the formula gives a(0) = 2. */
    if (n == 0)
        return (seq_t)2;

    /* Denominator: D = 2*n - (-1)^n + 3.
       For even n  : D = 2*n + 2
       For odd  n  : D = 2*n + 4 */
    uint64_t D = 2 * n + ((n & 1ULL) ? 4ULL : 2ULL);

    /* Compute 2^(n+1) mod D using binary exponentiation. */
    uint64_t pow = 1 % D;
    uint64_t base = 2 % D;
    idx_t exp = n + 1;          /* exponent may be large but fits in idx_t    */
    while (exp > 0) {
        if (exp & 1ULL)
            pow = (unsigned __int128)pow * base % D;
        base = (unsigned __int128)base * base % D;
        exp >>= 1ULL;
    }

    /* Compute r = (n mod D) * (2^(n+1) mod D) mod D. */
    uint64_t r = ((unsigned __int128)(n % D)) * pow % D;

    /* Decision logic based on the relation between r and D. */
    if (r < 2ULL)           /* r + 2 < D for n ≥ 1, so a(n) = 1          */
        return (seq_t)1;

    /* For r ≥ 2, a(n) = 1 if r + 2 ≥ D else 0. */
    if (r + 2ULL >= D)
        return (seq_t)1;
    else
        return (seq_t)0;
}
