/*
 * A356982  Fixed point of the morphism 0->010, 1->000.
 * https://oeis.org/A356982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A253786(n) mod 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the fixed point of the morphism
   0 → 010, 1 → 000.  The nth term (n ≥ 1) can be computed
   by examining the ternary representation of n‑1. */

seq_t a(idx_t n)
{
    /* The offset is 1.  The nth term is 0‑indexed as i = n‑1. */
    if (n == 0)          /* undefined for n < 1, but return 0. */
        return (seq_t)0;

    idx_t i = n - 1;     /* zero‑based index */
    seq_t val = 0;       /* start from the root symbol 0 */

    /* Find the highest power of 3 ≤ i. */
    idx_t pow = 1;
    while (pow <= i / 3) /* i/3 fits in idx_t */
        pow *= 3;

    while (pow > 0) {
        idx_t digit = i / pow;   /* most significant remaining digit */
        i %= pow;
        pow /= 3;

        if (val == 1) {
            /* parent is 1 → all children are 0. */
            val = 0;
        } else { /* parent is 0 */
            if (digit == 1)
                val = 1; /* only the middle child of a 0 becomes 1 */
            /* else val stays 0 */
        }
    }

    return val;
}
