/*
 * A381056  Product of row n of A329708.
 * https://oeis.org/A381056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..2*n} Sum_{i=max(0,k-n)}^{min(k,n)}
 *            (i+1)*(k-i+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A381056.
   The function follows the definition: a(n) = ∏_{k=0}^{2n} A329708(n,k),
   where   A329708(n,k) = ∑_{i=max(0,k-n)}^{min(k,n)} (i+1)*(k-i+1).   */
static seq_t term_sum(idx_t n, idx_t k)
{
    /* Determine the summation limits */
    idx_t L, U;
    if (k <= n) {
        L = 0;
        U = k;
    } else {
        L = k - n;
        U = n;
    }

    /* Variables for the closed‑form computation */
    idx_t A = L + 1;          /* first summand index (t=A) */
    idx_t B = U + 1;          /* last  summand index (t=B) */

    /* Σ t over [A,B]  */
    seq_t len   = (seq_t)(U - L + 1);          /* number of terms */
    seq_t sumT  = ((seq_t)(A + B) * len) / 2;  /* (A+B)*len/2 */

    /* Σ t² over [A,B]  using the closed form for ∑_{i=1}^{m} i² */
    seq_t up   = (seq_t)B      * (seq_t)(B + 1) * (seq_t)(2 * B + 1);
    seq_t down = (seq_t)(A - 1) * (seq_t)A      * (seq_t)(2 * (A - 1) + 1);
    seq_t sumT2 = (up - down) / 6;  /* (Σ_{1..B} i² - Σ_{1..A-1} i²) */

    /* Final term value: (k+2)*sumT - sumT2  */
    seq_t S = (seq_t)(k + 2) * sumT - sumT2;
    return S;
}

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t maxk = 2 * n;
    for (idx_t k = 0; k <= maxk; ++k)
        prod *= term_sum(n, k);
    return prod;
}
