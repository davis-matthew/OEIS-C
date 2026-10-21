/*
 * A279905  Number of 2 X 2 matrices with entries in {0,1,...,n} and odd trace
 *          with no elements repeated.
 * https://oeis.org/A279905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-2)*(n-1)*(2*(n+1)^2-(-1)^n-1))/4 for n>=0
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    /* Use 64‑bit arithmetic; for n up to ~10 000 the intermediate
       products fit in uint64_t (max 1e16 < 2^63). */
    uint64_t res32; /* holds the final 64‑bit result before cast */

    if ((n & 1ULL) == 0) {          /* n even */
        /* a(n) = ((n-2)*(n-1)*n*(n+2))/2 */
        uint64_t t1 = n - 2;
        uint64_t t2 = n - 1;
        uint64_t t3 = n;
        uint64_t t4 = n + 2;

        res32 = t1 * t2;
        res32 *= t3;
        res32 *= t4;
        res32 /= 2;
    } else {                        /* n odd */
        /* a(n) = ((n-2)*(n-1)*(n+1)*(n+1))/2 */
        uint64_t t1 = n - 2;
        uint64_t t2 = n - 1;
        uint64_t t3 = n + 1; /* squared twice */

        res32 = t1 * t2;
        res32 *= t3;
        res32 *= t3;
        res32 /= 2;
    }

    /* Convert the 64‑bit result to the build–specific seq_t type.
       All values for the requested n range fit in seq_t. */
    return (seq_t)res32;
}
