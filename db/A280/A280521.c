/*
 * A280521  From the "Fibonachos" game: Number of phases of the following
 *          routine: during each phase, the player removes objects from a pile
 *          of n objects as the Fibonacci sequence until the pile contains
 *          fewer objects than the next Fibonacci number. The phases continue
 *          until the pile is empty.
 * https://oeis.org/A280521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of phases of the Fibonachos routine described in
 *            A280521
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Remaining objects in the pile. */
    idx_t remaining = n;
    /* Count of phases executed. */
    idx_t phases = 0;

    while (remaining > 0) {
        /* Start a new phase. */
        phases++;

        /* Fibonacci numbers f1 = F1, f2 = F2. */
        idx_t f1 = 1;
        idx_t f2 = 1;

        /* Remove objects following the Fibonacci sequence until
           the next Fibonacci number would exceed the pile. */
        while (remaining >= f1) {
            remaining -= f1;
            idx_t next = f1 + f2;  /* f3 = f1 + f2 */
            f1 = f2;
            f2 = next;
        }
    }

    /* The phase count always fits into seq_t. */
    return (seq_t)phases;
}
