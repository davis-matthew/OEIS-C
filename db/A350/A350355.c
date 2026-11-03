/*
 * A350355  Numbers k such that the k-th composition in standard order is
 *          up/down.
 * https://oeis.org/A350355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the non‑negative integer k that is the nth composition,
 *             in standard order, which is up/down (alternating increasing
 *             and decreasing, starting with an increase).  The sequence
 *             values are the indices k, with a(1)=0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: determine whether the composition associated to k is up/down.
 * The composition is obtained from k by the bijection described in OEIS A066099.
 */
static bool is_up_down(uint64_t k)
{
    uint64_t parts[64];
    int count = 0;
    int pos = 1;            /* bit positions are 1‑based */
    uint64_t prev = 0;
    uint64_t tmp = k;

    while (tmp) {
        if (tmp & 1) {
            uint64_t diff = pos - prev;
            parts[count++] = diff;
            prev = pos;
        }
        tmp >>= 1;
        ++pos;
    }

    /* k == 0 produces an empty composition */
    if (count == 0)
        return true;

    /* reverse the differences to obtain the composition parts */
    for (int i = 0, j = count - 1; i < j; ++i, --j) {
        uint64_t t = parts[i];
        parts[i] = parts[j];
        parts[j] = t;
    }

    /* single part is always up/down */
    if (count == 1)
        return true;

    /* check alternating pattern */
    for (int i = 0; i < count - 1; ++i) {
        if ((i & 1) == 0) {          /* even index: must increase */
            if (!(parts[i] < parts[i + 1]))
                return false;
        } else {                     /* odd index: must decrease */
            if (!(parts[i] > parts[i + 1]))
                return false;
        }
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t found = 0;          /* how many up/down compositions found */
    uint64_t k = 0;           /* current index (non‑negative integer) */

    while (1) {
        if (is_up_down(k)) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        ++k;
    }
}
