/*
 * A336860  a(n) = 1 + the total remainder when repeatedly taking integer
 *          square roots until 1 is reached.
 * https://oeis.org/A336860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula implementation
   a(n) = 1 + sum of remainders while iterating
          n -> floor(sqrt(n)) -> … -> 1
   (i.e. a(n) = 1 + Σ (m - floor(sqrt(m))²) for each m > 1 in the chain)
*/

#include <stdint.h>

/* Integer square root of a 64‑bit value using binary search.
   The result is the largest integer r such that r*r <= n. */
static inline idx_t isqrt_u64(idx_t n)
{
    /* For zero or one we can return immediately */
    if (n < 2) return n;

    /* Upper bound for sqrt(2^64-1) is 2^32-1, so we can use 1ULL<<32 */
    idx_t low  = 0;
    idx_t high = (idx_t)1ULL << 32;   /* = 4294967296 */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        /* Use 128‑bit multiplication to avoid overflow */
        __uint128_t prod = (__uint128_t)mid * mid;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function: a(n) */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n==0 but avoid UB */
        return 0;

    seq_t total = 0;          /* will hold the accumulated remainders */
    idx_t m = n;              /* current value in the chain */

    while (m > 1) {
        idx_t r = isqrt_u64(m);                /* floor sqrt */
        seq_t rem = (seq_t)m - (seq_t)r * r;   /* remainder */
        total += rem;
        m = r;
    }

    return total + 1;
}
