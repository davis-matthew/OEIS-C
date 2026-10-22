/*
 * A360541  a(n) is the least number k such that k*n is a cubefull number
 *          (A036966).
 * https://oeis.org/A360541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A360541:  a(n) = ∏ₚ p^(max(e,3)−e) where n = ∏ₚ p^e, multiplicative.
   a(p)   = p²,  a(p²) = p,  a(p^e) = 1 for e ≥ 3.  The function works for
   any n ≥ 1; values below the offset are irrelevant. */

#include <stdint.h>

/* Return the least k such that k*n is cubefull (A036966). */
seq_t a(idx_t n)
{
    if (n == 0)           /* not in the intended range, but harmless */
        return 0;

    seq_t  result = 1;
    idx_t  rem    = n;

    /* Check for factor 2 separately to skip even numbers later. */
    if (rem % 2 == 0) {
        int exp = 0;
        while (rem % 2 == 0) {
            rem /= 2;
            ++exp;
        }
        if (exp == 1)            /* 2^1 → 2² */
            result = result * (seq_t)2 * (seq_t)2;
        else if (exp == 2)       /* 2² → 2¹ */
            result = result * (seq_t)2;
        /* exp ≥ 3 → factor contributes 1 */
    }

    for (idx_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            int exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            if (exp == 1)            /* p^1 → p² */
                result = result * (seq_t)p * (seq_t)p;
            else if (exp == 2)       /* p² → p¹ */
                result = result * (seq_t)p;
            /* exp ≥ 3 → contributes 1 */
        }
    }

    if (rem > 1) { /* remaining prime factor with exponent 1 */
        result = result * (seq_t)rem * (seq_t)rem;
    }

    return result;
}
