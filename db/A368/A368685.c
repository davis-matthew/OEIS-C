/*
 * A368685  a(n) = Product_{j=1..n, k=1..n} (j + k + n).
 * https://oeis.org/A368685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{j=1..n, k=1..n} (j + k + n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t ipow(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp > 0) {
        if (exp & 1) res *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t result = 1;
    idx_t two_n = 2 * n;          /* 2*n fits in idx_t (uint64_t) */
    idx_t n_plus_one = n + 1;

    for (idx_t s = 2; s <= two_n; ++s) {
        /* count(s) = number of pairs (j,k) with j+k=s */
        idx_t count = (s <= n_plus_one) ? (s - 1) : (two_n - s + 1);

        seq_t base = (seq_t)(s + n);   /* j + k + n = s + n */
        seq_t power = ipow(base, count);
        result *= power;
    }

    return result;
}
