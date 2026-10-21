/*
 * A252528  Number of (n+2) X (4+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 2 3 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-3) for n > 5. Initial values: a(1)=719, a(2)=462,
 *            a(3)=626, a(4)=790, a(5)=1676
 * Range:     u8 none, u16 n <= 13, u32 n <= 37, u64 n <= 85, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑compute the first five terms. */
    static const seq_t base[5] = {
        (seq_t)719,   /* a(1) */
        (seq_t)462,   /* a(2) */
        (seq_t)626,   /* a(3) */
        (seq_t)790,   /* a(4) */
        (seq_t)1676   /* a(5) */
    };

    if (n == 0)          /* 0 is outside the offset; return 0 safely. */
        return (seq_t)0;

    if (n <= 5)          /* directly return the known first five terms. */
        return base[n - 1];

    /* Find the smallest k ≤ n such that k ≤ 5 and k ≡ n (mod 3). */
    idx_t k = n;
    while (k > 5)
        k -= 3;

    seq_t value = base[k - 1];

    /* Multiply by 4 for every 3 steps from k up to n. */
    for (idx_t i = k; i < n; i += 3)
        value *= (seq_t)4;

    return value;
}
