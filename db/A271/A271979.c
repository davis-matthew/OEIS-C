/*
 * A271979  G_8(n), where G is the Goodstein function defined in A266201.
 * https://oeis.org/A271979
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = G_8(n) where G_8(n) denotes the 8‑th iterate of the
 *            Goodstein * function defined in OEIS A266201. The function
 *            follows the standard * hereditary base change process: each step
 *            replaces the current base b * by b+1 and then subtracts 1. The
 *            8th term is computed regardless of * the sequence l...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

/* Recursive helper: computes B_b(n) – the result of replacing every
 * occurrence of the base b by (b+1) in the hereditary base-b
 * representation of n.  B_b(n) equals n when n < b. */
static seq_t B_base(seq_t n, idx_t b)
{
    if (n < (seq_t)b) return n;

    seq_t result = 0;
    seq_t temp   = n;
    idx_t  pos   = 0;
    seq_t base_seq     = (seq_t)b;
    seq_t new_base_seq = (seq_t)(b + 1);

    while (temp != (seq_t)0) {
        seq_t digit = temp % base_seq;
        if (digit != (seq_t)0) {
            /* exponent for (b+1) is B_b(pos) */
            seq_t exp = B_base((seq_t)pos, b);
            seq_t pow_term = big_pow(new_base_seq, exp);
            result += digit * pow_term;
        }
        temp /= base_seq;
        ++pos;
    }
    return result;
}

/* Goodstein function G_8(n) */
seq_t a(idx_t n)
{
    seq_t value = (seq_t)n;
    const seq_t zero = (seq_t)0;

    for (int step = 1; step <= 8 && value != zero; ++step) {
        idx_t base = step + 1;                  /* 2,3,...,9 */
        seq_t bn = B_base(value, base);         /* change base */
        value = bn - (seq_t)1;                  /* subtract 1 */
    }
    return value;
}
