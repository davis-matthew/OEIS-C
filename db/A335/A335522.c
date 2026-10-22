/*
 * A335522  Numbers k such that the k-th composition in standard order
 *          (A066099) avoids the pattern (1,1,2).
 * https://oeis.org/A335522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   composition_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A335522:
   Numbers k such that the k‑th composition in standard order (A066099)
   avoids the pattern (1,1,2). */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/composition_k.h"

/* Return true iff the composition parts[0 … len-1] avoids the pattern (1,1,2).
   Pattern (1,1,2) means: there exist i<j<k with parts[i]==parts[j] and parts[k]>parts[i]. */
static bool avoid_pattern(const seq_t *parts, size_t len)
{
    if (len < 3)
        return true;

    for (size_t i = 0; i + 2 < len; ++i) {
        for (size_t j = i + 1; j + 1 < len; ++j) {
            if (parts[i] == parts[j]) {
                for (size_t k = j + 1; k < len; ++k) {
                    if (parts[k] > parts[i])
                        return false;      /* pattern found */
                }
            }
        }
    }
    return true;
}

/* Public function: a(n) – n‑th term of A335522 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined offset, safe return */

    idx_t count = 0;
    idx_t k = 0;
    seq_t value = 0;
    seq_t parts[128];
    size_t len;

    while (count < n) {
        composition_k(k, parts, &len);
        if (avoid_pattern(parts, len)) {
            ++count;
            if (count == n)
                value = (seq_t)k;
        }
        ++k;
    }
    return value;
}
