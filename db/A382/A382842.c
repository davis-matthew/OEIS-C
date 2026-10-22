/*
 * A382842  a(n) = Sum_{k=0..floor(n/2)} (binomial(n,k) * binomial(n-k,k))^3.
 * https://oeis.org/A382842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (binomial(n,k) * binomial(n-k,k))^3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A382842 */
/* a(n) = Σ_{k=0}^{⌊n/2⌋} (C(n,k) * C(n‑k,k))^3
   where C(a,b) = binomial coefficient */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = n / 2;

    /* C = C(n, k) iteratively: C(n,0)=1 */
    seq_t C = 1;
    /* B = C(n-k, k) iteratively. B(0)=1. */
    seq_t B = 1;

    for (idx_t k = 0; k <= limit; ++k) {
        seq_t prod = C * B;
        seq_t cube = prod * prod * prod;
        sum += cube;

        if (k == limit) break;           /* last term, no need to update further */

        /* Update C to C(n, k+1) = C(n, k) * (n-k) / (k+1) */
        C = C * (seq_t)(n - k) / (seq_t)(k + 1);

        /* Update B to C(n-(k+1), k+1)
           Derived recurrence: B_{k+1} = B_k * (n-2k) * (n-2k-1) / ((k+1)*(n-k))   */
        seq_t num = (seq_t)(n - 2 * k) * (seq_t)(n - 2 * k - 1);
        seq_t den = (seq_t)(k + 1) * (seq_t)(n - k);
        B = B * num / den;
    }

    return sum;
}
