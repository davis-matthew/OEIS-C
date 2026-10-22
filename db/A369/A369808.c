/*
 * A369808  Expansion of 1/(1 - x^5/(1-x)^7).
 * https://oeis.org/A369808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of 7*n-5 into parts 5 and 7 *
 *            (c(0)=1, c(m)=c(m-5)+c(m-7)). The array 'comp' stores * c(t) for
 *            all t needed, where t = 7*n-5
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *comp = NULL;   /* computed values for t = 0 .. max_t */
static idx_t comp_size = 0;  /* current number of stored entries */

/* Makes sure comp has entries 0 .. t.  If the array is enlarged it
   fills the new positions using the recurrence c(m)=c(m-5)+c(m-7). */
static void ensure_comp(idx_t t)
{
    if (t < comp_size)
        return;

    idx_t new_size = t + 1;
    seq_t *new_comp = (seq_t*)realloc(comp, new_size * sizeof(seq_t));
    if (!new_comp)
        exit(EXIT_FAILURE);
    comp = new_comp;

    for (idx_t i = comp_size; i < new_size; ++i) {
        seq_t val = 0;
        if (i >= 5) val += comp[i - 5];
        if (i >= 7) val += comp[i - 7];
        comp[i] = val;
    }
    comp_size = new_size;
}

/* a(n) – OEIS A369808. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Target index in the c‑table. */
    idx_t t = 7ULL * n - 5ULL;

    /* Initialise the table once. */
    if (!comp) {
        comp = (seq_t*)malloc(sizeof(seq_t));
        if (!comp)
            exit(EXIT_FAILURE);
        comp[0] = (seq_t)1;
        comp_size = 1;
    }

    ensure_comp(t);
    return comp[t];
}
