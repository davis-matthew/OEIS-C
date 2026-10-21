/*
 * A242179  T(0,0) = 1, T(n+1,2*k) = - T(n,k), T(n+1,2*k+1) = T(n,k), k=0..n,
 *          triangle read by rows.
 * https://oeis.org/A242179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, frac
 * Formula:   not stated
 * Range:     i8 n <= 8190, i16 n <= 8190, i32 n <= 8190, i64 n <= 8190,
 *            i128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A242179: Triangle defined by
   T(0,0) = 1,
   T(n+1,2k)   = -T(n,k),
   T(n+1,2k+1) =  T(n,k)
   Read by rows.
   a(n) is the n‑th element of the row‑major flattened triangle.
   The value is ±1.  It equals (-1)^{#zeros in the binary
   representation of k}, where k = n-(2^m-1) and
   m = floor(log2(n+1)).  Equivalently,
   a(n) = +1  if (m ^ popcount(k)) is even,
          -1  otherwise. */

#include <stdint.h>

/* Integer type supplied by the build */
typedef uint64_t idx_t;   /* remove these typedefs if seq_t/idx_t are already defined */

/* Helper: floor(log2(x)) for 0 < x < 2^64 */
static inline unsigned int floor_log2_u64(idx_t x)
{
    unsigned int r = 0;
    while (x >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* m = floor(log2(n+1)) */
    unsigned int m = floor_log2_u64(n + 1U);

    /* start index of row m: 2^m - 1 */
    idx_t start = ((idx_t)1 << m) - 1U;

    /* k is the index within the row */
    idx_t k = n - start;

    /* popcount of k */
    unsigned int pop = __builtin_popcountll(k);

    /* parity of number of zeros in an m‑bit representation of k:
       zeros = m - pop, so parity = (m ^ pop) & 1 */
    unsigned int parity = (m ^ pop) & 1;

    return parity ? (seq_t)-1 : (seq_t)1;
}
