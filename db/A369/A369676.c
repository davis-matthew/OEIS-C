/*
 * A369676  a(n) = Product_{k=0..n} (5^k + 5^(n-k)).
 * https://oeis.org/A369676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (5^k + 5^(n-k))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i;
    seq_t pow_n = 1;          /* 5^n */
    for (i = 0; i < n; ++i) pow_n *= 5;

    seq_t pow_fwd = 1;        /* 5^0 initially */
    seq_t pow_bwd = pow_n;    /* 5^n initially */
    seq_t prod   = 1;

    for (i = 0; i <= n; ++i) {
        prod *= (pow_fwd + pow_bwd);
        pow_fwd *= 5;
        if (i < n) pow_bwd /= 5;
    }

    return prod;
}
