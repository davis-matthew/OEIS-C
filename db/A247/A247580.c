/*
 * A247580  Base-19 state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (123...n)*.
 * https://oeis.org/A247580
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n * ord_n(19) , where ord_n(19) is the multiplicative
 *            order of 19 modulo n (the smallest positive k such that 19^k ≡ 1
 *            (mod n)). When 19 and n are not coprime (i.e. n % 19 == 0), the
 *            base‑19 alphabet is insufficient to distinguish all states, and
 *            the state complexity is n + 1. This matches...
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef BASE19_STATE_COMPLEXITY_H
#define BASE19_STATE_COMPLEXITY_H

#include <stdint.h>

/* Computes the base‑19 state complexity of a partitioned DFA for the
   periodic sequence (123...n)*.  The sequence is defined for n ≥ 2. */
seq_t a(idx_t n)
{
    /* The index is always ≥ 2; behaviour for smaller indices is
       unspecified. */
    if (n % 19 == 0)                /* 19 divides n → not coprime */
        return (seq_t)n + 1;

    /* Compute the multiplicative order of 19 modulo n by repeated
       multiplication.  Since 19 is prime and n ≤ 10 000 in typical
       test cases, this loop is trivial. */
    idx_t val = 1 % n;          /* start with 19^0 ≡ 1 (mod n) */
    idx_t k   = 0;
    do {
        val = (val * 19) % n;   /* 19^k (mod n) */
        ++k;
    } while (val != 1);

    /* a(n) = n * k */
    return (seq_t)n * (seq_t)k;
}

#endif /* BASE19_STATE_COMPLEXITY_H */
