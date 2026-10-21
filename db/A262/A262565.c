/*
 * A262565  A weaver's answer to the question "What comes next after 2,3,5?".
 * https://oeis.org/A262565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS sequence A262565: a repeating cycle of
   [2,3,5,5,3,2] for indices n ≥ 1 (offset 1). */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* tbl[i] gives a(1+i) for i = 0..5.  The sequence repeats every 6 terms. */
    static const seq_t tbl[6] = {
        (seq_t)2,
        (seq_t)3,
        (seq_t)5,
        (seq_t)5,
        (seq_t)3,
        (seq_t)2
    };
    return tbl[(size_t)((n - 1) % 6)];
}
