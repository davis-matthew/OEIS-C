/*
 * A340504  Fixed under 0 -> 03, 1 -> 23, 2 -> 21, 3 -> 01.
 * https://oeis.org/A340504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by walking the binary representation of n-1 *
 *            through the two‑uniform morphism 0→03, 1→23, 2→21, 3→01. * The
 *            most significant bit first determines whether we take the *
 *            first or second child of the current digit
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: when n = 1, n-1 = 0, the first digit is 0. */
    if (n <= 1)
        return (seq_t)0;

    idx_t i = n - 1;                 /* zero‑based index */
    /* Mapping tables for the morphism.
       Using unsigned char is safe because digits are in {0,1,2,3}. */
    static const unsigned char first_child[4]  = {0, 2, 2, 0};
    static const unsigned char second_child[4] = {3, 3, 1, 1};

    unsigned char d = 0;              /* start at the root digit 0 */

    /* Find the position of the most significant set bit of i.   */
    /* i != 0 here, so clzll is defined on a non‑zero value.      */
    int msb = 63 - __builtin_clzll((unsigned long long)i);

    /* Walk bits from most significant to least significant.      */
    for (int b = msb; b >= 0; --b) {
        if ((i >> b) & 1ULL)
            d = second_child[d];
        else
            d = first_child[d];
    }

    return (seq_t)d;
}
