/*
 * A254321  Hyper Wasteful numbers.
 * https://oeis.org/A254321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   add_digits_to_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Hyper Wasteful numbers: a(n) is the n‑th integer >1 such that
   D(n) > nbd(n), where D counts distinct digits in all prime factors
   and in their exponents > 1, and nbd counts distinct digits in n
   itself. This function computes the sequence by brute‑force testing
   each integer. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/add_digits_to_mask.h"

/* popcount for an unsigned int (only low 10 bits are used) */
static inline int popcount10(unsigned int m)
{
    int c = 0;
    while (m) {
        m &= m - 1;
        ++c;
    }
    return c;
}

/* seq_t a(idx_t n) as required by the evaluator */
seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t num = 2;                /* start testing from 2 */
    while (found < n) {
        unsigned int digit_mask = 0;
        uint64_t tmp = num;

        /* factor 2 separately */
        if (tmp % 2 == 0) {
            unsigned int exp = 0;
            while (tmp % 2 == 0) {
                tmp /= 2;
                ++exp;
            }
            add_digits_to_mask(2, &digit_mask);
            if (exp > 1)
                add_digits_to_mask(exp, &digit_mask);
        }

        /* trial division for odd factors */
        for (uint64_t d = 3; d * d <= tmp; d += 2) {
            if (tmp % d == 0) {
                unsigned int exp = 0;
                while (tmp % d == 0) {
                    tmp /= d;
                    ++exp;
                }
                add_digits_to_mask(d, &digit_mask);
                if (exp > 1)
                    add_digits_to_mask(exp, &digit_mask);
            }
        }

        /* remaining prime factor > 1 */
        if (tmp > 1) {
            add_digits_to_mask(tmp, &digit_mask);
            /* exponent is 1, so nothing added for exponent */
        }

        int D = popcount10(digit_mask);

        /* distinct digits in the number itself */
        unsigned int n_mask = 0;
        add_digits_to_mask(num, &n_mask);
        int nbd = popcount10(n_mask);

        if (D > nbd) {
            ++found;
            if (found == n)
                return (seq_t)num;
        }
        ++num;
    }
    return (seq_t)0;   /* should never reach here for n >= 1 */
}
