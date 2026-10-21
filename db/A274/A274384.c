/*
 * A274384  Numbers n such that 2^n is not the average of three positive
 *          cubes.
 * https://oeis.org/A274384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274384: numbers n such that 3*2^n is NOT a sum of three positive cubes.
   The set is finite: an integer n < 47 is in the sequence iff
     (n%3 == 1)  OR  (n%3 == 2 and n <= 32).
   This reproduces all 27 terms known to exist (1,2,4,5,7,8,10,11,13,14,16,17,19,20,
   22,23,25,26,28,29,31,32,34,37,40,43,46). All other n >= 47 are representable
   and thus not in the sequence. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t terms[27];
    static size_t len = 0;
    static bool init = false;

    if (!init) {
        size_t idx = 0;
        for (idx_t cand = 1; cand < 47; ++cand) {
            /* Determine whether cand is in the sequence */
            if ((cand % 3u) == 1u ||
                ((cand % 3u) == 2u && cand <= 32u)) {
                terms[idx++] = (seq_t)cand;
            }
        }
        len = idx;            /* should be 27 */
        init = true;
    }

    if (n == 0 || n > len) {
        return (seq_t)0;      /* undefined for out of range */
    }
    return terms[n - 1];
}
