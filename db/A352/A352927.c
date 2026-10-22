/*
 * A352927  Numbers whose digits are nonzero, consecutive, and all increasing
 *          or all decreasing.
 * https://oeis.org/A352927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   cmp_seq_t_func, generate_sequence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A352927: numbers whose digits are nonzero, consecutive, and either
   all increasing or all decreasing.  The function generates all 81 such
   numbers, sorts them, and returns the n‑th (1‑based) value. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_seq_t_func.h"
#include "../utils/generate_sequence.h"

/* External-linkage function implementing the sequence */
seq_t a(idx_t n)
{
    static seq_t seq[81];
    static bool initialized = false;
    static size_t count = 0;

    if (!initialized) {
        generate_sequence(seq, &count);
        qsort(seq, count, sizeof(seq_t), cmp_seq_t_func);
        initialized = true;
    }

    if (n == 0) return 0;                 /* undefined for offset 0 */
    idx_t idx = n - 1;
    if ((size_t)idx >= count) return 0;    /* out of range */
    return seq[idx];
}
