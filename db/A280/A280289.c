/*
 * A280289  Numbers n such that number of partitions of n is odd and number of
 *          partitions of n into distinct parts is even.
 * https://oeis.org/A280289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS A280289.
   The term is the n‑th positive integer n such that
   - the partition function p(n) is odd,
   - the number of partitions of n into distinct parts q(n) is even.
   The routine computes the parities of p(k) and q(k) for all k up
   to a dynamically chosen limit (5*n+100) using simple dynamic
   programming modulo 2, then scans for the n‑th qualifying number. */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* safety for very small n */
    if (n == 0) return 0;

    idx_t limit = 5 * n + 100;          /* generous upper bound */
    uint8_t *par_p = (uint8_t*)calloc(limit + 1, 1);
    uint8_t *par_q = (uint8_t*)calloc(limit + 1, 1);
    if (!par_p || !par_q) return 0;    /* allocation failure */

    /* parity of unrestricted partitions p(k)   */
    par_p[0] = 1;                       /* p(0) = 1 (even parity considered 1) */
    for (idx_t k = 1; k <= limit; ++k) {
        for (idx_t j = k; j <= limit; ++j) {
            par_p[j] ^= par_p[j - k];
        }
    }

    /* parity of partitions into distinct parts q(k)  */
    par_q[0] = 1;
    for (idx_t k = 1; k <= limit; ++k) {
        for (idx_t j = limit; j >= k; --j) {
            par_q[j] ^= par_q[j - k];
        }
    }

    /* collect the n‑th qualifying index */
    idx_t count = 0;
    seq_t result = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (par_p[i] && !par_q[i]) {   /* p(i) odd AND q(i) even */
            ++count;
            if (count == n) {
                result = (seq_t)i;
                break;
            }
        }
    }

    /* free memory */
    free(par_p);
    free(par_q);

    return result;
}
