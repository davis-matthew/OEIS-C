/*
 * A272090  Number of chiral partitions of n; number of irreducible
 *          representations of the symmetric group S_n with nontrivial
 *          determinant.
 * https://oeis.org/A272090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 64, u32 n <= 480, u64 n <= 4088,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A272090: a(n) = A000041(n)-A045923(n).
   Computation by binary expansion of n:
   Let n = e + Σ 2^k_i (k_i increasing), e = 0 or 1.
   Then
     a(n) = 2^{Σ_{i=2..r} k_i} *
            ( 2^{k1-1}
              + Σ_{v=1}^{k1-1} 2^{(v+1)(k1-2)-v(v-1)/2}
              + e·2^{k1(k1-1)/2} ).
   a(1) = 0.  The formula works for all n ≥ 1. */

static seq_t _pow2(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = (idx_t)0; i < exp; ++i)
        r <<= 1;          /* shift by 1 repeatedly to avoid UB for large exp */
    return r;
}

seq_t a(idx_t n)
{
    if (n <= (idx_t)1)  /* a(1) = 0, and a(0) is unused but safe to return 0 */
        return (seq_t)0;

    idx_t e = n & (idx_t)1;          /* least significant bit */
    idx_t m = n >> 1;                /* remaining bits correspond to exponents >0 */

    idx_t k1 = 0;                    /* smallest exponent >0 */
    idx_t sum_rest = 0;              /* sum of exponents excluding the smallest */
    idx_t pos = 0;                   /* position of bit in m (0-based) */

    while (m != (idx_t)0)
    {
        if ((m & (idx_t)1) != (idx_t)0)
        {
            idx_t exp = pos + (idx_t)1;   /* exponent in original n */
            if (k1 == (idx_t)0)
                k1 = exp;                /* first exponent */
            else
                sum_rest += exp;
        }
        m >>= 1;
        ++pos;
    }

    /* outer factor 2^{sum_rest} */
    seq_t outer = (sum_rest == (idx_t)0) ? (seq_t)1 : _pow2(sum_rest);

    /* inner sum */
    seq_t inner = (seq_t)0;

    /* term 2^{k1-1} */
    inner += _pow2(k1 - (idx_t)1);

    /* sum over v = 1 .. k1-1 of 2^{(v+1)(k1-2)-v(v-1)/2} */
    for (idx_t v = (idx_t)1; v < k1; ++v)
    {
        idx_t exp =
            (v + (idx_t)1) * (k1 - (idx_t)2)
            - (v * (v - (idx_t)1)) / (idx_t)2;
        inner += _pow2(exp);
    }

    /* e part: e * 2^{k1(k1-1)/2} */
    if (e != (idx_t)0)
    {
        idx_t exp_e = (k1 * (k1 - (idx_t)1)) / (idx_t)2;
        inner += _pow2(exp_e);
    }

    return outer * inner;
}
