/*
 * A265008  Number of positive solutions to the equation A x B = C, where A, B
 *          and C are made from (contiguous) substrings of the binary
 *          representation of n.
 * https://oeis.org/A265008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 312, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A265008: number of positive solutions A × B = C where A, B and C
   are obtained from distinct positive integer values of contiguous
   substrings of the binary representation of n.  Distinct pairs (A,B)
   with A ≠ B are counted twice (A×B and B×A), while A×A is counted once. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Determine bit length of n (must be ≥1). */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        len++;
        tmp >>= 1;
    }

    /* Extract the bits of n into an array (MSB first). */
    uint8_t bits[64];
    for (idx_t i = 0; i < len; i++) {
        bits[i] = (uint8_t)((n >> (len - 1 - i)) & 1u);
    }

    /* Collect all distinct positive values obtainable from substrings.
       For 64 bits the most substrings is 64*65/2 = 2080, so 2100 is safe. */
    seq_t values[2100];
    size_t sz = 0U;  /* number of distinct values stored */

    for (idx_t i = 0; i < len; i++) {
        seq_t val = 0;
        for (idx_t j = i; j < len; j++) {
            val = (val << 1) + (seq_t)bits[j];
            if (val == 0)
                continue;          /* ignore zero substrings */

            /* Check for duplicates. */
            bool dup = false;
            for (size_t k = 0; k < sz; k++) {
                if (values[k] == val) { dup = true; break; }
            }
            if (!dup) {
                values[sz++] = val;
            }
        }
    }

    /* Count solutions A × B = C with A,B,C in the set. */
    seq_t result = 0;
    for (size_t cidx = 0; cidx < sz; cidx++) {
        seq_t c = values[cidx];
        for (size_t aidx = 0; aidx < sz; aidx++) {
            seq_t a_val = values[aidx];
            if (c % a_val == 0) {
                seq_t b = c / a_val;
                /* Verify that b is also in the set. */
                for (size_t k = 0; k < sz; k++) {
                    if (values[k] == b) {
                        result++;
                        break;
                    }
                }
            }
        }
    }

    return result;
}
