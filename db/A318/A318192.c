/*
 * A318192  a(n) = U_{n}(n)/(n+1) where U_{n}(x) is a Chebyshev polynomial of
 *          the second kind.
 * https://oeis.org/A318192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (1/(2*k+1)) *
 *            binomial(n,2*k)*(n^2-1)^k*n^(n-2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A318192.
 * It uses an efficient recurrence for the summand that avoids storing
 * giant intermediate powers.  For 64‑bit builds a 128‑bit intermediate
 * type is used to keep all intermediate values exact.  For arbitrary
 * precision builds the sequence is computed using the free‑form
 * operations available on seq_t.  */

seq_t a(idx_t n)
{
    /* a(0) and a(1) are trivial */
    if (n == 0) return 1;
    if (n == 1) return 1;

    /* ----------------- 64‑bit build with 128‑bit intermediate  ----------------- */
#if defined(__SIZEOF_INT128__)
    if (sizeof(seq_t) == 8) {
        typedef unsigned __int128 u128;

        /* term_0 = n^n  */
        u128 term = 1;
        for (idx_t i = 0; i < n; ++i) term *= (u128)n;

        /* sum = term_0  */
        u128 sum = term;

        u128 n2   = (u128)n * n;
        u128 n2m1 = n2 - 1;

        /* compute successive terms using the recurrence
         * term_{k+1} = term_k *
         *   (n-2k)(n-2k-1)(n^2-1) /
         *   ((2k+2)(2k+3)n^2)   */
        for (idx_t k = 0; k < n/2; ++k) {
            u128 num = (u128)(n - 2*k) * (u128)(n - 2*k - 1) * n2m1;
            u128 den = (u128)(2*k + 2) * (u128)(2*k + 3) * n2;
            term = term * num / den;
            sum += term;
        }
        return (seq_t)sum;   /* sum fits in 64 bits */
    }
#endif

    /* -----------------  Arbitrary‑precision or non‑128‑bit  builds  ----------------- */
    /* term_0 = n^n  */
    seq_t term = 1;
    seq_t base = (seq_t)n;
    for (idx_t i = 0; i < n; ++i) term *= base;

    /* sum = term_0  */
    seq_t sum = term;

    seq_t n2   = base * base;
    seq_t n2m1 = n2 - 1;

    for (idx_t k = 0; k < n/2; ++k) {
        seq_t num = (seq_t)(n - 2*k) * (seq_t)(n - 2*k - 1);
        num *= n2m1;
        seq_t den = (seq_t)(2*k + 2) * (seq_t)(2*k + 3);
        den *= n2;
        term = term * num / den;
        sum += term;
    }

    return sum;   /* the sum already equals a(n) */
}
