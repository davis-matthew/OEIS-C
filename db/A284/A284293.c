/*
 * A284293  Numbers using only digits 1 and 6.
 * https://oeis.org/A284293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 54, u32 n <= 1534, u64 n <= 9499,
 *            u128 n <= 9499, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A284293: numbers using only digits 1 and 6, ordered by
 * increasing number of digits and then lexicographically.  The nth term
 * is obtained by taking the zero‑based index within its digit length
 * block, interpreting the binary representation of that index,
 * mapping 0 → 1 and 1 → 6, and constructing the corresponding decimal
 * integer. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, just in case */

    idx_t cumulative = 0;   /* total numbers in all shorter lengths */
    unsigned len = 1;       /* current digit length */

    /* Find the digit length that contains the nth term. */
    while (1) {
        idx_t group = (idx_t)1 << len;          /* 2^len numbers of this length */
        if (n <= cumulative + group) break;    /* desired length found */
        cumulative += group;
        len++;
    }

    idx_t pos = n - 1 - cumulative;            /* position within the block (0‑based) */
    seq_t  val = 0;                             /* resulting decimal number */

    /* Build the number by traversing bits from most significant to least. */
    for (unsigned i = 0; i < len; ++i) {
        unsigned shift = len - 1 - i;
        unsigned bit   = (pos >> shift) & 1;
        val = val * 10 + (bit ? 6 : 1);
    }

    return val;
}
