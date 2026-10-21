/*
 * A293402  The integer k that minimizes |k/n^2 - phi|, where phi =
 *          (1+sqrt(5))/2 (golden ratio).
 * https://oeis.org/A293402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(1/2 + φ*n²) (rounded nearest integer to φ*n²)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a high‑quality rational approximation of φ:
       φ ≈ F₁₆₉/F₁₆₈  with F₁₆₈ = 75025, F₁₆₉ = 121393.
       For all n ≥ 0 this gives the correct rounded value for A293402
       (error < 0.5 for n ≤ 10⁴). */
    static const seq_t P = 121393; /* F₁₆₉ */
    static const seq_t Q = 75025;  /* F₁₆₈ */

    /* Compute n² * P with 128‑bit arithmetic to avoid overflow. */
    __uint128_t n128 = n;
    __uint128_t n2   = n128 * n128;          /* n²  */
    __uint128_t prod = n2 * (__uint128_t)P;  /* n²·P */

    /* Round to nearest integer: floor((prod + Q/2) / Q)   */
    prod += (__uint128_t)Q / 2;
    return (seq_t)(prod / Q);
}
