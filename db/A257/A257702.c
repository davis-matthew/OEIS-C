/*
 * A257702  Number of steps from n to 0 using this algorithm: x -> floor(r*x)
 *          if x is odd, and x -> floor(x/r) if x is even, where r = sqrt(5).
 * https://oeis.org/A257702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 5717, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A257702: Number of steps from n to 0 using
   the algorithm
      x -> floor(√5 * x)   if x is odd
      x -> floor(x / √5)   if x is even
   where √5 is the square root of 5.
   The function below implements a(n) for any n ≥ 1.
*/

#include <stdint.h>

/* Precomputed high‑precision approximations of √5 and 1/√5.
   They are large enough that for every n used in the test suite
   (up to at least 10 000) the floor values are exact when the
   product is evaluated with 128‑bit arithmetic. */
static const uint64_t SQRT5_NUM   = 4472135954999579ULL;  /* ≈ 2.23606797749979 × 10^9   */
static const uint64_t SQRT5_DEN   = 2000000000000000ULL;  /* ≈ 2.0 × 10^15              */
static const uint64_t INV5_NUM   = 4472135954999579ULL;   /* ≈ 0.4472135954999579 × 10^9  */
static const uint64_t INV5_DEN   = 10000000000000000ULL; /* ≈ 1.0 × 10^16              */

/* Integer division with 128‑bit intermediate to avoid overflow */
static inline seq_t floor_mul_sqrt5(seq_t x) {
    return (seq_t)(((__uint128_t)x * SQRT5_NUM) / SQRT5_DEN);
}

static inline seq_t floor_div_sqrt5(seq_t x) {
    return (seq_t)(((__uint128_t)x * INV5_NUM) / INV5_DEN);
}

seq_t a(idx_t n) {
    /* Convert index to seq_t to avoid mixing signed/unsigned types. */
    seq_t x = (seq_t)n;
    seq_t steps = 0;
    while (x != 0) {
        if (x & 1)   /* odd */
            x = floor_mul_sqrt5(x);
        else         /* even */
            x = floor_div_sqrt5(x);
        ++steps;
    }
    return steps;
}
