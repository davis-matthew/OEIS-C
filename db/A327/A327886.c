/*
 * A327886  Digits d in decimal expansion of n replaced with d-th digit of n
 *          (keeping the digits 0). If n does not have enough digits to index,
 *          the indexing resumes at the first digit of n as many times as
 *          necessary to find the substitution digit. Leading zeros are erased
 *          unless the result is 0.
 * https://oeis.org/A327886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look, base
 * Formula:   not stated
 * Range:     u8 n <= 229, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A327886 – Digit substitution
 *
 *  For a non‑negative integer n, write its decimal expansion n = d1 d2 … dk
 *  (d1 is the most significant digit).  The value a(n) is obtained by
 *  replacing each digit di with the digit that occurs at position di in
 *  the same expansion, treating the positions cyclically.
 *
 *  If a digit equals 0 it is left unchanged (i.e. 0→0).  The resulting
 *  sequence of digits is interpreted as an integer; any leading zeros
 *  are discarded unless the entire result is 0.
 *
 *  Example:
 *    n = 2056748 → digits 2,0,5,6,7,4,8
 *    substitutes → 0,0,7,4,8,6,2 → integer 74862.
 *
 *  This function implements the above rule.  It runs in O(number of
 *  decimal digits of n) time and uses only the permitted arithmetic
 *  operators on seq_t and idx_t.  It works for the built‑in integer
 *  types as well as for arbitrary‑precision seq_t when compiled as
 *  C++17.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert n to an array of its decimal digits (most significant first). */
    unsigned char rev[21];          /* buffer for reversed digits */
    size_t revlen = 0;

    if (n == 0) {
        rev[0] = 0;
        revlen = 1;
    } else {
        uint64_t tmp = n;
        while (tmp > 0) {
            rev[revlen++] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }
    }

    unsigned char digits[21];
    size_t len = revlen;
    for (size_t i = 0; i < revlen; ++i)
        digits[i] = rev[revlen - 1 - i];

    /* Apply the substitution rule. */
    seq_t result = 0;
    bool leading = true;   /* leading zeros not yet output */
    bool allzero = true;   /* track if result is entirely zero */

    for (size_t i = 0; i < len; ++i) {
        unsigned char d = digits[i];
        unsigned char sub;

        if (d == 0)
            sub = 0;
        else
            sub = digits[(size_t)(d - 1) % len];

        if (leading && sub == 0)
            continue;      /* skip leading zeros */

        leading = false;
        allzero = false;
        result = result * 10 + sub;
    }

    return allzero ? (seq_t)0 : result;
}
