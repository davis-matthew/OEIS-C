/*
 * A345414  a(n) = n^a(n-1) mod 100; a(0) = 0.
 * https://oeis.org/A345414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = n^a(n-1) mod 100, a(0) = 0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t mod_pow(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1;
    base %= mod;
    /* the exponent a(n-1) is always ≤ 94, so a simple loop is fine */
    while (exp > 0) {
        result = (result * base) % mod;
        exp -= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* For n = 0 we are given a(0) = 0 */
    if (n == 0) return 0;

    /* The sequence satisfies a(n + 100) = a(n), so only the residue mod 100 matters */
    idx_t m = n % 100;

    /* Compute the values from 0 up to m */
    seq_t values[101];
    values[0] = 0;
    for (idx_t i = 1; i <= m; ++i) {
        values[i] = mod_pow((seq_t)i, values[i - 1], 100);
    }

    return values[m];
}
