/*
 * A382433  a(n) = S(6,n), where S(r,n) = Sum_{k=0..floor(n/2)} (
 *          binomial(n,k) - binomial(n,k-1) )^r.
 * https://oeis.org/A382433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} ( binomial(n,k) - binomial(n,k-1)
 *            )^6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Sum over k = 0 .. floor(n/2) of (C(n,k) - C(n,k-1))^6
       We compute binomials iteratively to avoid factorials. */
    idx_t limit = n / 2;
    seq_t sum = 0;
    seq_t c = 1;          /* C(n,0) */
    seq_t prev = 0;       /* C(n,-1) = 0 */

    for (idx_t k = 0; k <= limit; ++k) {
        seq_t B = c - prev;          /* binomial difference */
        /* compute B^6 by repeated multiplication */
        seq_t p = 1;
        for (int i = 0; i < 6; ++i) p *= B;
        sum += p;

        /* update for next k if any */
        if (k == limit) break;
        /* C(n,k+1) = C(n,k) * (n - k) / (k + 1) */
        seq_t next = c * (seq_t)(n - k);
        next /= (seq_t)(k + 1);
        prev = c;
        c = next;
    }
    return sum;
}
