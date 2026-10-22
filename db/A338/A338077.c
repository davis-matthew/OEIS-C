/*
 * A338077  Diagonal terms in the expansion of (1+x*y+y*z+z*x)/(1-x-y-z).
 * https://oeis.org/A338077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n-1) * (3*n)! / ((3*n-1) * n!^3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* which is equivalent to a(n) = (4*n-1)/(3*n-1) * C(3*n,n) * C(2*n,n) */

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 u128;
#else
typedef uint64_t u128;
#endif

/* multiplies a by mul and divides by div using a u128 temporay to avoid overflow.
   If div == 1 the division is omitted. */
static inline seq_t mul_div(seq_t a, seq_t mul, seq_t div)
{
#if defined(__SIZEOF_INT128__)
    u128 tmp = (u128)a * (u128)mul;
    return (seq_t)(tmp / div);
#else
    return (seq_t)((a * mul) / div);
#endif
}

/* Computes binomial coefficient C(n,k) for k <= n using multiplicative formula.
   Result fits in seq_t because the sequence a(n) is only required for values that
   fit in the current seq_t type. */
static inline seq_t binomial(idx_t n, idx_t k)
{
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = mul_div(r, (seq_t)(n - k + i), (seq_t)i);
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* compute C(3n,n) and C(2n,n) */
    seq_t c3n_n = binomial(3 * n, n);
    seq_t c2n_n = binomial(2 * n, n);

    /* a(n) = (4n-1)/(3n-1) * c3n_n * c2n_n */
    seq_t result = c3n_n;
    result = mul_div(result, (seq_t)(4 * n - 1), 1);        /* multiply by 4n-1 */
    result = mul_div(result, c2n_n, (seq_t)(3 * n - 1));    /* multiply by c2n_n and divide by (3n-1) */
    return result;
}
