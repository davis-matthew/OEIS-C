/*
 * A397556  Sequence where k is appended after every k^3 occurrences of 1,
 *          with multiple values following a 1 listed in order.
 * https://oeis.org/A397556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A397556: a(n) = 1, except after each block of k³ consecutive 1s a
   k is appended immediately afterwards.  The terms are generated incrementally:
   for every new 1 the counter of 1s is increased; then every k (k ≥ 2) with
   k³ | counter is appended in ascending order. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *data = NULL;   /* cached sequence terms             */
    static size_t len = 0;       /* number of terms currently stored  */
    static size_t cap = 0;       /* allocated capacity                */
    static idx_t ones = 0;       /* count of ones produced so far     */

    if (n == 0)                /* n ≥ 1 by definition, safeguard */
        return (seq_t)0;

    /* Extend the cached sequence until it contains at least n terms */
    while (len < (size_t)n) {
        /* Ensure capacity for the upcoming 1 */
        if (len >= cap) {
            size_t new_cap = cap ? cap * 2 : 64;
            data = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
            cap = new_cap;
        }
        data[len++] = (seq_t)1;  /* append a 1 */
        ones++;                  /* increase the global counter of 1s */

        /* Append every k with k³ dividing the current counter */
        for (idx_t k = 2;; ++k) {
            idx_t cube = k * k * k;
            if (cube > ones) break;
            if (ones % cube == 0) {
                if (len >= cap) {
                    size_t new_cap = cap ? cap * 2 : 64;
                    data = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
                    cap = new_cap;
                }
                data[len++] = (seq_t)k;
            }
        }
    }

    return data[n - 1];
}
