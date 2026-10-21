/*
 * A285384  Limiting 1-word of the morphism 0 -> 11, 1 -> 01.
 * https://oeis.org/A285384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth symbol (1‑indexed) of the limiting word of the
 *            morphism 0 → 11, 1 → 01. The value is obtained by traversing the
 *            substitution tree from the appropriate depth down to the root.
 *            No closed form is known
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive helper: returns the symbol at position "pos" (1‑indexed) in the
   word produced after 'depth' applications of the morphism.  */
static inline seq_t
get_symbol(idx_t depth, idx_t pos)
{
    /* Base word S0 = "0" */
    if (depth == 0)
        return (seq_t)0;

    /* Parent position in the previous word (ceil(pos / 2)) */
    idx_t parent = (pos + 1) >> 1;
    seq_t parent_sym = get_symbol(depth - 1, parent);

    /* Map according to the morphism rules */
    if (parent_sym == 0)            /* 0 → 11 */
        return (seq_t)1;
    else /* parent_sym == 1 */      /* 1 → 01 */
        return (pos & 1) ? (seq_t)0 : (seq_t)1;
}

seq_t a(idx_t n)
{
    /* The first term is defined to be 1. */
    if (n == 1)
        return (seq_t)1;

    /* Find the smallest k ≥ 1 such that 2^k ≥ n. */
    idx_t k = 0;
    idx_t lim = (idx_t)1;
    while (lim < n) {
        lim <<= 1;
        ++k;
    }
    if (k == 0)
        k = 1;           /* Ensure depth ≥ 1 for n > 1 */

    return get_symbol(k, n);
}
