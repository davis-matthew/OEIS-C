/*
 * A264263  The number of distinct nontrivial integral cevians of an isosceles
 *          triangle, with base of length 1 and legs of length n, that divide
 *          the base into two integral parts.
 * https://oeis.org/A264263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integers d ≥ 2 such that n^2 + d^2 – d is a
 *            perfect square
 * Range:     u8 n <= 9, u16 n <= 225, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (equivalently, the number of nontrivial integer cevians of an isosceles triangle with
 *    legs of length n and base 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* m = 4*n^2 – 1  is odd; every solution corresponds to a factor pair (a,b) with
     * a * b = m, a ≤ b, a,b odd, and a,b satisfy the parity conditions below. */
    seq_t nn = (seq_t)n;
    seq_t m  = 4 * nn * nn - 1;

    seq_t count = 0;

    /* iterate all odd divisors a of m with a ≤ sqrt(m) */
    for (idx_t a = 1; (seq_t)a * (seq_t)a <= m; a += 2) {
        if (m % (seq_t)a != 0) continue;                /* not a divisor          */
        idx_t b = (idx_t)(m / (seq_t)a);                /* complementary divisor   */
        if ((b & 1) == 0) continue;                    /* b must be odd          */
        if (a > b) continue;                           /* ensure a ≤ b           */

        seq_t diff = (seq_t)b - (seq_t)a;               /* = 2y                   */
        if (diff & 1) continue;                        /* diff must be even      */
        seq_t y = diff / 2;                            /* y = (b – a)/2 → (2d‑1) */

        if (y == 0) continue;                          /* d would be 0.5         */

        seq_t sum = (seq_t)a + (seq_t)b;                /* = 2x * 2 = ...         */
        if ((sum & 3) != 0) continue;                  /* sum must be multiple of 4 */

        /* k = sum/4 is integer; d = (y+1)/2 is integer            */
        seq_t d = (y + 1) / 2;                         /* candidate cevian length*/
        if (d < 2) continue;                          /* exclude trivial (d=0,1) */

        /* n^2 + d^2 – d is automatically a square by construction */
        count++;
    }

    return count;
}
