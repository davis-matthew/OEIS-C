/*
 * A281367  "Nachos" sequence based on triangular numbers.
 * https://oeis.org/A281367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 925, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = number of phases needed to empty a pile of n nachos
 *              by repeatedly subtracting successive triangular numbers
 *              t_k = k*(k+1)/2 from the pile until the next triangular
 *              exceeds the remaining nachos, then starting a new phase.
 *              This is the definition of OEIS sequence A281367. */
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t remaining = (seq_t)n;           /* remaining nachos */
    seq_t phases = 0;                    /* count of phases */

    while (remaining > 0) {
        phases++;                        /* start a new phase */

        /* subtract successive triangular numbers until we cannot */
        for (idx_t i = 1; ; ++i) {
            seq_t t = (seq_t)i * (i + 1) / 2;  /* triangular number t_i */
            if (t > remaining)
                break;
            remaining -= t;
        }
    }

    return phases;
}
