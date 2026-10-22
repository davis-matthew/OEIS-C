/*
 * A322249  a(n) = A322248(n)^2, the square of the central coefficient in (1 +
 *          5*x + 16*x^2)^n.
 * https://oeis.org/A322249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( ∑_{k=0}^n (-3)^{n‑k} * 4^k * C(n,k) * C(2k,k) )^2 The
 *            implementation uses only integer arithmetic, with a single O(n)
 *            loop. It avoids the need for the helper function `big_pow`. The
 *            code works for all builds where `seq_t` is any unsigned type
 *            (including arbitrary precision) and where `id...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Single precision accumulators for the two partial sums. */
    seq_t pos = 0;  /* sum of terms with a positive sign   */
    seq_t neg = 0;  /* sum of terms with a negative sign   */

    /* Binomial coefficient C(n,k), initially C(n,0) = 1. */
    seq_t comb_nk = 1;
    /* Binomial coefficient C(2k,k), initially C(0,0) = 1. */
    seq_t comb_2k_k = 1;
    /* Power of 4: 4^k, initially 1. */
    seq_t pow4 = 1;
    /* Power of 3: 3^(n‑k).  Compute 3^n once, then divide by 3 each step. */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= (seq_t)3;

    /* Sign for k = 0:   (-3)^n = (-1)^n * 3^n. */
    int sign = ((n & 1) == 0) ? 1 : -1;

    /* Term for k = 0. */
    seq_t term = comb_nk;
    term *= comb_2k_k;
    term *= pow4;
    term *= pow3;

    if (sign > 0)
        pos += term;
    else
        neg += term;

    /* Iterate k = 0 .. n‑1 to compute the term for k+1. */
    for (idx_t k = 0; k < n; ++k) {
        /* Update C(n,k+1) from C(n,k).  Division is exact. */
        comb_nk *= (seq_t)(n - k);
        comb_nk /= (seq_t)(k + 1);

        /* Update C(2(k+1),k+1) from C(2k,k).  Division is exact. */
        comb_2k_k *= (seq_t)(2 * k + 2);
        comb_2k_k *= (seq_t)(2 * k + 1);
        comb_2k_k /= (seq_t)(k + 1);
        comb_2k_k /= (seq_t)(k + 1);

        /* Update the powers. */
        pow4 *= (seq_t)4;
        pow3 /= (seq_t)3;       /* 3^(n-(k+1)) */

        /* Flip the sign for the next term. */
        sign = -sign;

        /* Compute the term for k+1. */
        term = comb_nk;
        term *= comb_2k_k;
        term *= pow4;
        term *= pow3;

        if (sign > 0)
            pos += term;
        else
            neg += term;
    }

    /* The central coefficient is the difference of the two sums. */
    seq_t central = pos - neg;   /* pos ≥ neg for all n ≥ 0. */
    /* Return the square of the central coefficient. */
    return central * central;
}
