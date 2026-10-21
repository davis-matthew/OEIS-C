/*
 * A258736  Number of length n+6 0..3 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A258736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{11} c_i * binom(n-1-i+7, 7) , (n ≥ 1) where
 *            binom(m,7) = m*(m-1)*…*(m-6) / 5040 and c_i are the coefficients
 *            of the numerator in the generating function
 * Range:     u8 none, u16 n <= 2, u32 n <= 70, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* coefficients from the OEIS entry (degree 11 polynomial) */
static const int64_t coef[12] = {
    16384, -87391, 177424, -140720, -31344,
     116775, -39024, -13988, -31192,  58867,
    -31680,   5890
};

/* Computes binom(m, 7) for m ≥ 7 using 128‑bit arithmetic */
static seq_t binom7(idx_t m)
{
    unsigned __int128 prod = 1;
    for (idx_t k = 0; k < 7; ++k)
        prod *= (unsigned __int128)(m - k);
    prod /= (unsigned __int128)5040;   /* 7! = 5040 */
    return (seq_t)prod;
}

/* OEIS sequence A258736 */
seq_t a(idx_t n)
{
    if (n == 0U)   /* undefined for n < 1, return 0 just in case */
        return 0U;

    /* offset n by 1: formula uses n-1 as the base index */
    idx_t base = n - 1U;

    signed __int128 result = 0;
    for (int i = 0; i < 12; ++i) {
        if (base < (idx_t)i)
            continue;                /* binom(m,7) equals 0 when m < 7 */
        idx_t m = base - (idx_t)i + 7U;   /* m ≥ 7 for these terms */
        seq_t bin = binom7(m);
        result += (signed __int128)coef[i] * (signed __int128)bin;
    }
    return (seq_t)result;
}
