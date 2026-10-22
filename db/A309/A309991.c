/*
 * A309991  Balanced quinary (base 5) enumeration (or balanced quinary
 *          representation) of integers, write n in quinary, and then replace
 *          3's with (-2)'s and 4's with (-1)'s.
 * https://oeis.org/A309991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 129, i16 n <= 9176, i32 n <= 9176, i64 n <= 9176,
 *            i128 n <= 9176, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced Quinary Representation
 * a(n) = Σ m_i * 5^i
 * where n is expressed in base‑5 as Σ d_i * 5^i,
 * and each digit d_i is mapped:
 *   0 → 0, 1 → 1, 2 → 2, 3 → -2, 4 → -1
 * This yields a bijection between the non‑negative integers
 * and all integers. */

#include <stdint.h> /* for uint64_t */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t place  = 1;
    idx_t temp   = n;

    while (temp > 0) {
        uint64_t d = temp % 5ULL;   /* current base‑5 digit */
        temp /= 5ULL;              /* next digit */

        seq_t mapped;
        if (d < 3)          /* 0,1,2 */
            mapped = d;
        else if (d == 3)    /* 3 → -2 */
            mapped = - (seq_t)2;
        else                /* 4 → -1 */
            mapped = - (seq_t)1;

        result += mapped * place;
        place *= 5;
    }

    return result;
}
