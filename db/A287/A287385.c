/*
 * A287385  Start with 0 and repeatedly substitute 0->012, 1->210, 2->021.
 * https://oeis.org/A287385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) is the nth term of the fixed point of the morphism
 *   0 -> 012
 *   1 -> 210
 *   2 -> 021
 *   starting with the symbol 0.
 */
seq_t a(idx_t n)
{
    /* mapping[parent][child] gives the child symbol produced by the
       replacement of parent with 3 symbols.  All values are in {0,1,2}. */
    static const unsigned char mapping[3][3] = {
        {0, 1, 2},
        {2, 1, 0},
        {0, 2, 1}
    };

    /* Find the smallest power of 3 that is >= n.  This is the length
       of the word at the level that contains position n. */
    idx_t len = 1;
    while (len < n) {
        len *= 3;
    }

    /* Convert to a 0‑based index. */
    idx_t pos = n - 1;

    /* Start from symbol 0. */
    unsigned char symbol = 0;

    /* Walk from the top level down to the leaf, updating the symbol
       according to the base‑3 expansion of pos. */
    while (len > 1) {
        len /= 3;
        unsigned char digit = (unsigned char)(pos / len);
        pos %= len;
        symbol = mapping[symbol][digit];
    }

    return (seq_t)symbol;
}
