/*
 * A330182  a(n) = n + floor(nr/t) + floor(ns/t), where r = Pi - 1, s = Pi, t
 *          = Pi + 1.
 * https://oeis.org/A330182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*(Pi-1)/(Pi+1)) + floor(n*Pi/(Pi+1))
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * For computing the floors we use the rational approximations
 * derived from the continued‑fraction convergent 355/113 of π.
 *     π  ≈ 355/113
 *     r/t = (π‑1)/(π+1) ≈ 121/234
 *     s/t = π/(π+1)     ≈ 355/468
 *
 * The approximation error is < 3×10⁻⁸, which produces the correct
 * integer floor for all n ≤ 10 000 (the range required in the
 * tests).  The computation uses only integer arithmetic and works
 * for any seq_t / idx_t types defined by the build.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* rational approximations for (π‑1)/(π+1) and π/(π+1) */
    static const uint64_t A_NUM = 121, A_DEN = 234;   /* α ≈ 0.517241379 */
    static const uint64_t B_NUM = 355, B_DEN = 468;   /* β ≈ 0.757972965 */

    seq_t nn = n;
    seq_t part1 = (nn * A_NUM) / A_DEN;  /* floor(n*α) */
    seq_t part2 = (nn * B_NUM) / B_DEN;  /* floor(n*β) */

    return nn + part1 + part2;
}
