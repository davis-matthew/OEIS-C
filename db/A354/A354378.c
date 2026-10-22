/*
 * A354378  a(n) is the denominator of Sum_{k=0..n} (-1)^k / (2*k)!.
 * https://oeis.org/A354378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A354378
    a(n) = denominator of   S_n = Σ_{k=0..n} (-1)^k / (2k)!.
    Let N_n = Σ_{k=0..n} (-1)^k * (2n)!/(2k)!   (always >0)
    Then    a(n) = (2n)! / gcd( N_n , (2n)! )
    Moreover N_n satisfies   N_0 = 1,
          N_k = (2k)(2k-1) * N_{k-1} + (-1)^k   (k ≥ 1). */

/* Helper: Euclidean gcd for arbitrary‑precision unsigned type seq_t */
static inline seq_t _gcd_seq(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* S_0 = 1  →  den = 1 */
        return 1;

    seq_t N = 1;  /* N_0 */
    seq_t D = 1;  /* (2⋅0)! = 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* d = (2k)(2k-1)  (fits into 32‑bit, cast to seq_t) */
        seq_t k2 = (seq_t)k * 2;
        seq_t d  = k2 * (k2 - 1);

        /* Update N_k :  N = d*N + (-1)^k  */
        if ((k & 1u) == 0)
            N = N * d + 1;      /* even k → +1 */
        else
            N = N * d - 1;      /* odd  k → -1 */

        /* Update full denominator D = (2k)!  */
        D = D * d;
    }

    /* Final denominator after reducing the fraction N/D */
    seq_t g = _gcd_seq(N, D);
    return D / g;
}
