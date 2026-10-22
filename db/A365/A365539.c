/*
 * A365539  Array read by ascending antidiagonals: A(n,k) = [x^n] (1 +
 *          x^k)/((1 - x)^2*(1 - x^k)), with k > 0.
 * https://oeis.org/A365539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A365539 – read by ascending antidiagonals of
   A(n,k) = [x^n] (1 + x^k) / ((1 - x)^2 * (1 - x^k)), k > 0 */

static inline seq_t compute_A(idx_t n, idx_t k)
{
    /* compute A(n,k) using the closed‑form derived below */
    seq_t n_seq = (seq_t)n;
    seq_t k_seq = (seq_t)k;

    idx_t m = n / k;               /* floor(n/k) */
    seq_t m_seq = (seq_t)m;

    /* first sum over j = 0 .. floor(n/k) */
    seq_t term1 = (m_seq + 1) * (n_seq + 1) -
                  k_seq * (m_seq * (m_seq + 1) / 2);

    /* second sum (only when n >= k) */
    seq_t term2 = 0;
    if (n >= k) {
        idx_t m1 = (n - k) / k;     /* floor((n-k)/k) = floor(n/k)-1 for n>=k */
        seq_t m1_seq = (seq_t)m1;
        term2 = (m1_seq + 1) * (n_seq - k_seq + 1) -
                k_seq * (m1_seq * (m1_seq + 1) / 2);
    }

    return term1 + term2;
}

seq_t a(idx_t n)
{
    /* locate the (i,k) pair on the antidiagonal that contains index n */
    idx_t cumulative = 0;
    idx_t d = 0;
    while (1) {
        cumulative += d + 1;        /* number of elements up to diagonal d */
        if (cumulative > n) break; /* found the diagonal containing n */
        ++d;
    }

    idx_t offset = n - (cumulative - (d + 1)); /* position inside the diagonal */
    idx_t i = d - offset;
    idx_t k = offset + 1;

    return compute_A(i, k);
}
