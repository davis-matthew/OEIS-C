/*
 * A275962  Total number of nonzero digits that occur on the multiply occupied
 *          slopes of the factorial base representation of n: a(n) =
 *          A275812(A275734(n)). (See comments for more exact definition).
 * https://oeis.org/A275962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of occurrences of duplicate (position - digit)
 *            values in factorial base representation of n
 * Range:     u8 n <= 9972, u16 n <= 9972, u32 n <= 9972, u64 n <= 9972,
 *            u128 n <= 9972, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute a(n) as defined in OEIS A275962. */
seq_t a(idx_t n)
{
    /* Temporary array to hold the diff values.
       The factorial representation of a 64‑bit integer needs at most 20 digits
       (20! > 2^64). */
    idx_t diffs[32];
    size_t count = 0;

    idx_t temp = n;
    idx_t pos  = 1;               /* position counted from the right, starting at 1 */
    while (temp > 0) {
        idx_t base = pos + 1;     /* digit base is (pos+1) */
        idx_t digit = temp % base;
        temp /= base;
        if (digit > 0) {
            /* Only non‑zero digits contribute a diff. */
            diffs[count++] = pos - digit;
        }
        ++pos;
    }

    /* Sort the diff array.  The array length is tiny (≤ 20),
       so a simple insertion sort is sufficient. */
    for (size_t i = 1; i < count; ++i) {
        idx_t key = diffs[i];
        size_t j = i;
        while (j > 0 && diffs[j - 1] > key) {
            diffs[j] = diffs[j - 1];
            --j;
        }
        diffs[j] = key;
    }

    /* Count duplicate occurrences. */
    seq_t total = 0;
    for (size_t i = 0; i < count; ) {
        size_t j = i + 1;
        while (j < count && diffs[j] == diffs[i])
            ++j;
        size_t freq = j - i;
        if (freq > 1)
            total += (seq_t)freq;  /* add all occurrences that are part of duplicates */
        i = j;
    }

    return total;
}
