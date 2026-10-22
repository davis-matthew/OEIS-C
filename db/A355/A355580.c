/*
 * A355580  Powerful 3-smooth numbers: numbers of the form 2^i * 3^j with i, j
 *          != 1.
 * https://oeis.org/A355580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th strictly increasing number of the form 2^i * 3^j
 *            with i,j ≠ 1 (offset 1). The sequence is closed under
 *            multiplication
 * Range:     u8 n <= 14, u16 n <= 70, u32 n <= 299, u64 n <= 1242,
 *            u128 n <= 5065, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                      /* offset 0 is undefined, harmless */

    /* Persistent state for generating the sequence lazily. */
    static seq_t *seq      = NULL;          /* 1‑based array of terms */
    static idx_t  count    = 0;             /* number of terms already generated */
    static size_t capacity = 0;             /* allocated size of seq (without a[0]) */
    static idx_t idx4  = 1;                 /* pointer for factor 4   */
    static idx_t idx8  = 1;                 /* pointer for factor 8   */
    static idx_t idx9  = 1;                 /* pointer for factor 9   */
    static idx_t idx27 = 1;                 /* pointer for factor 27  */

    /* First call: allocate initial storage and seed the first term. */
    if (count == 0) {
        capacity = 1024;
        seq = (seq_t *)malloc((capacity + 1) * sizeof(seq_t));
        if (!seq)
            exit(EXIT_FAILURE);
        seq[1] = (seq_t)1;                  /* a(1) = 1 */
        count = 1;
    }

    /* Grow array if more space is needed for the requested index. */
    while (capacity < (size_t)n) {
        capacity <<= 1;
        seq = (seq_t *)realloc(seq, (capacity + 1) * sizeof(seq_t));
        if (!seq)
            exit(EXIT_FAILURE);
    }

    /* Generate terms until the array contains at least n elements. */
    while (count < n) {
        seq_t cand4  = seq[idx4] * (seq_t)4;
        seq_t cand8  = seq[idx8] * (seq_t)8;
        seq_t cand9  = seq[idx9] * (seq_t)9;
        seq_t cand27 = seq[idx27] * (seq_t)27;

        seq_t next = cand4;
        if (cand8 < next) next = cand8;
        if (cand9 < next) next = cand9;
        if (cand27 < next) next = cand27;

        seq[++count] = next;

        if (cand4  == next) ++idx4;
        if (cand8  == next) ++idx8;
        if (cand9  == next) ++idx9;
        if (cand27 == next) ++idx27;
    }

    return seq[n];
}
