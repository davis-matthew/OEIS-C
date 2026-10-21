/*
 * A277572  (1/2)*A277568.
 * https://oeis.org/A277572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n-th positive integer that is 1 or 4 modulo 6, or a multiple of 6 whose
   quotient is also an element of the sequence.  This implements OEIS A277572.
   The value is computed recursively by scanning the positive integers, keeping
   a lookup table of already chosen terms (needed for the 6×quotient rule).    */
#include <stdlib.h>
#include <stdint.h>

/* NOTE: seq_t and idx_t types are provided by the build system. */
seq_t a(idx_t n)
{
    /* Local dynamic array acting as a boolean lookup for included numbers. */
    uint8_t *included = NULL;      /* 0 = not included, 1 = included     */
    size_t inc_size = 0;           /* current capacity of 'included' array */

    idx_t count = 0;          /* number of terms found so far    */
    idx_t m = 1;              /* current integer being examined  */

    /* Helper lambda to ensure the table can store index 'idx'. */
    #define ENSURE_CAPACITY(idx)                     \
        do {                                         \
            if ((size_t)(idx) >= inc_size) {         \
                size_t new_size = (idx) + 1;          \
                uint8_t *tmp = (uint8_t*)realloc(included, new_size); \
                if (!tmp) return 0; /* allocation failure */ \
                /* zero the newly allocated region */  \
                for (size_t i = inc_size; i < new_size; ++i) tmp[i] = 0; \
                included = tmp;                         \
                inc_size = new_size;                    \
            }                                          \
        } while (0)

    /* Initialize lookup for index 0 (unused but convenient) */
    ENSURE_CAPACITY(0);

    while (count < n) {
        int cur_included = 0;
        if (m % 6 == 1 || m % 6 == 4) {
            cur_included = 1;
        } else if (m % 6 == 0) {                 /* multiple of 6 */
            idx_t q = m / 6;
            ENSURE_CAPACITY(q);
            cur_included = included[q] ? 1 : 0;
        }

        if (cur_included) {
            ENSURE_CAPACITY(m);
            included[m] = 1;
            ++count;
            if (count == n)
                return (seq_t)m;
        }

        ++m;
    }

    /* Should never be reached */
    return 0;
}
