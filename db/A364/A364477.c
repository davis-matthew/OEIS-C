/*
 * A364477  G.f. satisfies A(x) = 1 + x*A(x)^2 + x^2*A(x)^7.
 * https://oeis.org/A364477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..⌊n/2⌋} binomial(2*n+3*k, k) * binomial(2*n+2*k,
 *            n-2*k) / (n+4*k+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binomial_calc(idx_t n, idx_t k)
{
    /* Reduce k to at most n/2 to minimise the number of iterations */
    if (k > n - k)
        k = n - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* result = result * (n-k+i) / i   (exact division) */
        result = result * (seq_t)(n - k + i) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t b1 = binomial_calc(2 * n + 3 * k, k);
        seq_t b2 = binomial_calc(2 * n + 2 * k, n - 2 * k);
        idx_t   denom = n + 4 * k + 1;
        seq_t prod;

        /* Use 128‑bit intermediate when seq_t fits in 64 bits. */
        if (sizeof(seq_t) <= 8)
        {
            unsigned __int128 temp = (unsigned __int128)b1 *
                                    (unsigned __int128)b2;
            temp /= denom;            /* denom is an integer */
            prod = (seq_t)temp;
        }
        else
        {
            prod = b1 * b2 / denom;   /* seq_t may be arbitrary precision */
        }

        sum += prod;
    }

    return sum;
}
