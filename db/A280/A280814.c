/*
 * A280814  The maximum number of squares among the partial sums of any
 *          permutation of the integers [1..n].
 * https://oeis.org/A280814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n + n mod 2 + (n+1/2)*sqrt(2))/4)
 * Range:     u8 n <= 52, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation evaluates the conjectured closed form
    using only integer arithmetic. */

/* Use the provided integer types. */
#include <stdint.h>

/* Compute integer square root of an unsigned __int128 value.
   It returns floor(sqrt(x)).  This routine is safe for any
   non‑negative x that fits in 128 bits. */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 r = 0;
    /* bitwise linear search for the most significant bit of sqrt(x) */
    for (int i = 127; i >= 0; --i) {
        unsigned __int128 t = r | ((unsigned __int128)1 << i);
        /* Compare t*t <= x without overflow: t <= x / t  */
        if (t <= x / t) {
            r = t;
        }
    }
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Guard against the (unlikely) case n == 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* k = 2*n + 1  (odd integer) */
    idx_t k = 2 * n + 1;

    /* floor(k*sqrt(2)) = floor(sqrt(2*k^2)) */
    unsigned __int128 kk = (unsigned __int128)k * k;   /* k^2         */
    unsigned __int128 val = kk * 2;                   /* 2*k^2       */
    unsigned __int128 floor_k128 = isqrt128(val);      /* integer sqrt */
    seq_t floor_k = (seq_t)floor_k128;                /* cast to seq_t */

    /* Compute A = 2*n + 2*(n%2) + floor_k */
    seq_t two_n      = (seq_t)(2 * n);
    seq_t parity_term = (seq_t)(2 * (n & 1));        /* n%2 as 0 or 1 */
    seq_t A = two_n + parity_term + floor_k;

    /* Result is floor(A / 8) */
    seq_t result = A / (seq_t)8;
    return result;
}
