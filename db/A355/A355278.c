/*
 * A355278  Lower left of the Cayley table for the primes when made into a
 *          group using the bijection (2, 3, 5, 7, ...) -> (0, +1, -1, +2,
 *          ...) into (Z, +); read by rows.
 * https://oeis.org/A355278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A355278 – lower left of the Cayley table of primes under the
    bijection 2→0 , 3→+1 , 5→−1 , 7→+2 , … with (Z,+).
    Computes the n‑th element by
      1. determining the pair (m,n) in the lower‐triangle
      2. converting each index to its integer value,
      3. summing the two integers,
      4. converting that sum back to a prime index,
      5. returning the prime via get_prime.
*/

#include "../utils/get_prime.h"

/* map prime index (1‑based) to its integer in the bijection */
static long long map_index(idx_t idx)
{
    if (idx == 1) return 0;
    idx_t n = idx - 2;            /* zero based after the first prime */
    idx_t n1 = n >> 1;            /* floor((idx-2)/2) */
    long long mag = (long long)n1 + 1;   /* magnitude */
    if ((idx & 1) == 0) return mag;     /* even idx → + */
    else                 return -mag;   /* odd  idx → − */
}

seq_t a(idx_t n)
{
    /* Determine the row m and column j from n.
       The lower left triangle contains 1,2,3,5,6,9,… entries.
       The cumulative count up to row m is m*(m+1)/2. */
    idx_t m = 1;
    idx_t tri = 1;                /* current triangular number   */
    while (tri < n) {
        ++m;
        tri += m;                 /* tri = m*(m+1)/2 now           */
    }
    idx_t prev_tri = tri - m;     /* (m-1)*m/2   */
    idx_t j = n - prev_tri;       /* column index 1‑based          */

    /* Convert indices to integers */
    long long mi = map_index(m);
    long long mj = map_index(j);
    long long k = mi + mj;        /* integer sum under +   */

    /* Convert integer back to prime index */
    idx_t prime_idx;
    if (k == 0) {
        prime_idx = 1;            /* 2  */
    } else if (k > 0) {
        prime_idx = (idx_t)k * 2;        /* 2*k   */
    } else {
        prime_idx = (idx_t)(-k) * 2 + 1; /* -2*k+1 */
    }

    return (seq_t)get_prime(prime_idx);
}
