/*
 * A387551  a(n) = (1/2) * Sum_{k=0..floor(n/2)} 2^k *
 *          binomial(2*k+2,2*n-4*k+1).
 * https://oeis.org/A387551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2) * Sum_{k=0..floor(n/2)} 2^k * binomial(2*k+2,
 *            2*n-4*k+1)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (for n >= 6): a(n) = 4*a(n-2) + 4*a(n-3) - 4*a(n-4) + 8*a(n-5) - 4*a(n-6) */

seq_t a(idx_t n)
{
    /* Base terms a(0)..a(5) */
    static const seq_t base[6] = { (seq_t)1, (seq_t)0, (seq_t)4, (seq_t)4, (seq_t)12, (seq_t)40 };

    if (n < 6)
        return base[n];

    /* Circular buffer storing the last six values a(n-6)..a(n-1) */
    seq_t val[6];
    val[0] = base[0];
    val[1] = base[1];
    val[2] = base[2];
    val[3] = base[3];
    val[4] = base[4];
    val[5] = base[5];

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute 4*a(i-2) + 4*a(i-3) + 8*a(i-5) */
        seq_t t1 = val[(i - 2) % 6] << 2;          /* 4*a(i-2) */
        t1 += val[(i - 3) % 6] << 2;              /* + 4*a(i-3) */
        t1 += val[(i - 5) % 6] << 3;              /* + 8*a(i-5) */

        /* Compute 4*a(i-4) + 4*a(i-6) */
        seq_t t2 = val[(i - 4) % 6] << 2;          /* 4*a(i-4) */
        t2 += val[(i - 6) % 6] << 2;              /* + 4*a(i-6) */

        val[i % 6] = t1 - t2;                     /* a(i) */
    }

    return val[n % 6];
}
