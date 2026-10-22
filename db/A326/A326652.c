/*
 * A326652  a(n) is the sum of each distinct digit in n, raised to the power
 *          of its number of appearances in n.
 * https://oeis.org/A326652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{d∈digits(n)} d^{cnt_d(n)}
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the sum of each distinct digit in n raised to the power of its number of appearances in n. */
seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 */
    if (n == 0) return 0;

    /* Count the occurrences of each decimal digit */
    idx_t temp = n;
    int cnt[10] = {0};
    for (; temp != 0; temp /= 10)
    {
        int d = (int)(temp % 10);
        cnt[d]++;
    }

    /* Compute the sum of digit^count for each non‑zero count */
    seq_t result = 0;
    for (int d = 0; d < 10; ++d)
    {
        int c = cnt[d];
        if (c == 0)
            continue;

        seq_t base = (seq_t)d;
        seq_t power = (seq_t)1;
        for (int i = 0; i < c; ++i)
            power *= base;

        result += power;
    }

    return result;
}
