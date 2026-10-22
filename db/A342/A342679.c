/*
 * A342679  Number of steps for n to reach 1 or n by repeated application of
 *          A037916, or -1 if they are never reached.
 * https://oeis.org/A342679
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   Number of steps for n to reach 1 or n by repeated application of
 *            A037916 (concatenation of exponents of the prime factorisation)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   digit_len_uint, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Note: seq_t and idx_t are provided by the build system.
    The function works for any n ≥ 2.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len_uint.h"

/* Main function implementing the sequence. */
seq_t a(idx_t n)
{
    /* We only care about n >= 2; caller guarantees it. */
    seq_t orig = n;
    seq_t cur   = n;
    seq_t steps = 0;

    /* Safety limit to break potential cycles (not required for known range). */
    const unsigned int MAX_STEPS = 64;

    while (steps < MAX_STEPS) {
        /* Factorise cur, build the concatenated exponent number. */
        uint64_t u = (uint64_t)cur;          /* cur fits in 64 bits for all cases. */
        seq_t next = 0;

        for (uint64_t p = 2; p <= u / p; ++p) {
            if (u % p == 0) {
                unsigned int exp = 0;
                while (u % p == 0) {
                    u   /= p;
                    exp += 1;
                }
                unsigned int d = digit_len_uint(exp);
                next = next * pow10((idx_t)d) + exp;
            }
        }
        if (u > 1) {               /* remaining prime factor with exponent 1 */
            unsigned int d = digit_len_uint(1);
            next = next * pow10((idx_t)d) + 1;
        }

        steps++;
        cur = next;

        if (cur == 1 || cur == orig)
            return steps;
    }

    /* If we exceeded MAX_STEPS, we treat it as a non‑terminating orbit. */
    return (seq_t)-1;
}
