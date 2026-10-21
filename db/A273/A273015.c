/*
 * A273015  Ramanujan's largely composite numbers having 3 as the greatest
 *          prime divisor.
 * https://oeis.org/A273015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   ceil_log_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A273015: Ramanujan's largely composite numbers having 3
   as the greatest prime divisor. It is obtained by listing all
   numbers of the form 2^a * 3^b with 0 <= a <= 5, 1 <= b <= 3
   that satisfy a >= b-1, and sorting them in ascending order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ceil_log_pow.h"

seq_t a(idx_t n) {
    /* Static storage for the sorted sequence */
    static seq_t *seq = NULL;
    static idx_t len = 0;

    /* Compute sequence once */
    if (!seq) {
        /* Exponents bounds from the theorem: 2*ceil(log_p(next_prime)) - 1 */
        const unsigned int next_prime = 5;          /* next prime after 3 */
        const unsigned int max_a = 2 * ceil_log_pow(2, next_prime) - 1; /* 5 */
        const unsigned int max_b = 2 * ceil_log_pow(3, next_prime) - 1; /* 3 */

        /* Total possible candidates */
        const size_t max_candidates = (size_t)(max_a + 1) * (size_t)(max_b);

        seq = (seq_t *)malloc(max_candidates * sizeof(seq_t));
        if (!seq) return 0;          /* allocation failure */

        /* Pre‑compute powers of 3 */
        seq_t pow3[4] = {1, 3, 9, 27};

        size_t count = 0;
        for (unsigned int b = 1; b <= max_b; ++b) {
            unsigned int a_start = (b > 1) ? b - 1 : 0;
            for (unsigned int a = a_start; a <= max_a; ++a) {
                seq_t val = ((seq_t)1 << a) * pow3[b];
                seq[count++] = val;
            }
        }

        /* Sort the values in ascending order */
        for (size_t i = 1; i < count; ++i) {
            seq_t key = seq[i];
            size_t j = i;
            while (j > 0 && seq[j - 1] > key) {
                seq[j] = seq[j - 1];
                --j;
            }
            seq[j] = key;
        }

        len = (idx_t)count;
    }

    /* Return the nth element (1‑based).  Return 0 if n is out of range. */
    if (n == 0) return 0;
    idx_t idx = n - 1;
    if (idx >= len) return 0;
    return seq[idx];
}
