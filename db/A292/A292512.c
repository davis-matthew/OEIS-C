/*
 * A292512  Sequence A: Start with n, add the sum of digits of n (A062028) and
 *          repeat. Sequence B: Start with n, add the sum of base-100 digits
 *          of n and repeat. a(n) is the smallest common number > n.
 * https://oeis.org/A292512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9868 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

/* Helper: sum of base‑100 digits of x. */
static inline seq_t digit_sum_base100(seq_t x) {
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 100;
        x /= 100;
    }
    return sum;
}
/* Method: two‑pointer search for the smallest common value > n
   in Sequence A: add decimal digit sum (A062028)
   and Sequence B: add base‑100 digit sum.  */

/* Return the smallest common value > n in the two sequences. */
seq_t a(idx_t n)
{
    /* Convert index to seq_t for calculations. */
    seq_t curA = (seq_t)n;
    seq_t curB = (seq_t)n;

    /* Advance both sequences to the first term strictly > n. */
    if (curA <= (seq_t)n) {
        curA += digit_sum(curA);
    }
    if (curB <= (seq_t)n) {
        curB += digit_sum_base100(curB);
    }

    /* Merge-like search until the two values coincide. */
    while (curA != curB) {
        if (curA < curB) {
            curA += digit_sum(curA);
        } else {
            curB += digit_sum_base100(curB);
        }
    }
    return curA;   /* or curB, they are equal. */
}
