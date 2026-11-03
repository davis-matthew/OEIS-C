/*
 * A293872  Numbers having '12' as a substring of their digits.
 * https://oeis.org/A293872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 2989, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9953 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A293872: numbers whose decimal representation contains the
 * substring "12".
 * The function builds the sequence lazily; on each call it ensures that
 * at least the first `n` terms are computed and stored in a static array.
 * Subsequent calls reuse the already computed terms.
 */
#include <stdlib.h>
#include "../utils/mem.h"

/* Return 1 if x contains the decimal substring "12".  */
static int contains_12(seq_t x)
{
    int prev = -1;                 /* previous (lower) digit, or -1 initially */
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)10);   /* current (lower) digit */
        if (d == (int)1 && prev == (int)2) {
            return 1;             /* found substring "12" */
        }
        prev = d;
        x /= (seq_t)10;
    }
    return 0;
}

seq_t a(idx_t n)
{
    /* Lazy incremental construction of the sequence. */
    static seq_t *seq = NULL; /* array of computed terms */
    static size_t cap = 0;    /* allocated capacity of seq       */
    static size_t len = 0;    /* number of terms currently ready */

    if (n < (idx_t)1)          /* undefined for n < 1 – return 0 as sentinel */
        return (seq_t)0;

    if (n > (idx_t)len) {      /* need to compute more terms            */
        /* Grow the array to hold at least n terms. */
        size_t needed = (size_t)n;
        if (cap < needed) {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            while (new_cap < needed)
                new_cap *= 2;
            seq = mem_resize(seq_t, seq, new_cap);
            cap = new_cap;
        }

        seq_t candidate = (len == 0) ? (seq_t)12 : seq[len - 1] + (seq_t)1;
        while (len < needed) {
            if (contains_12(candidate))
                seq[len++] = candidate;
            candidate += (seq_t)1;
        }
    }

    return seq[n - 1];
}
