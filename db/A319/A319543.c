/*
 * A319543  a(n) = 1*2*3 - 4*5*6 + 7*8*9 - 10*11*12 + 13*14*15 - ... + (up to
 *          n).
 * https://oeis.org/A319543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k=1..⌈n/3⌉} (-1)^{k-1} * ∏_{i=3(k-1)+1}^{min(3k,n)} i
 * Range:     i8 n <= 8, i16 n <= 41, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the alternating sum of products of consecutive natural numbers in groups of three. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t i = 1;          /* current start of the group */
    int add = 1;          /* 1 => add the product, -1 => subtract */

    while (i <= n) {
        seq_t p = (seq_t)i;      /* product of the group, at most three factors */
        if (i + 1 <= n) p *= (seq_t)(i + 1);
        if (i + 2 <= n) p *= (seq_t)(i + 2);

        if (add > 0)
            total += p;
        else
            total -= p;

        i += 3;               /* move to the next group */
        add = -add;           /* toggle sign */
    }

    return total;
}
