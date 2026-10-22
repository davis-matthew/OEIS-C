/*
 * A320529  Number of chiral pairs of color patterns (set partitions) in a row
 *          of length n using exactly 6 colors (subsets).
 * https://oeis.org/A320529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (S2(n,6) - A(n,6)) / 2
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  S2(n,k) is Stirling subset number, A(n,k) satisfies the recurrence
    A(n,k) = k*A(n-2,k) + A(n-2,k-1) + A(n-2,k-2)  (n>1)
    with base A(0,0)=1, A(1,1)=1, other values zero. */

seq_t a(idx_t n) {
    const int K = 6;
    if (n == 0) return 0; /* not defined by offset, but safe */

    /* Stirling numbers S2(n, k) for k=0..6 */
    seq_t S[K+1];
    /* A(n,k) values for even and odd n parity */
    seq_t A_even[K+1], A_odd[K+1];

    /* initialise arrays with zeros */
    for (int i = 0; i <= K; ++i) {
        S[i]    = 0;
        A_even[i] = 0;
        A_odd[i]  = 0;
    }

    /* base cases */
    S[0] = 1;            /* S2(0,0) = 1 */
    A_even[0] = 1;       /* A(0,0) = 1 */
    A_odd[1] = 1;        /* A(1,1) = 1 */

    /* iterate from n=1 up to target n */
    for (idx_t cur = 1; cur <= n; ++cur) {
        /* update Stirling numbers using recurrence */
        for (int k = K; k >= 1; --k) {
            S[k] = k * S[k] + S[k-1];
        }
        S[0] = 0;   /* S2(n,0) = 0 for n>0 */

        /* compute A(cur, k) only for cur > 1 */
        if (cur >= 2) {
            int parity = (int)(cur & 1);   /* 0 for even, 1 for odd */
            seq_t *src = (parity == 0) ? A_even : A_odd;
            seq_t tmp[K+1];
            for (int i = 0; i <= K; ++i) {
                seq_t val = src[i] * (seq_t)i;
                if (i >= 1) val += src[i-1];
                if (i >= 2) val += src[i-2];
                tmp[i] = val;
            }
            /* copy updated values back to the correct parity array */
            if (parity == 0) {
                for (int i = 0; i <= K; ++i) A_even[i] = tmp[i];
            } else {
                for (int i = 0; i <= K; ++i) A_odd[i]  = tmp[i];
            }
        }
    }

    /* obtain A(n,6) based on parity */
    seq_t A_n_k = ((n & 1) ? A_odd[K] : A_even[K]);

    /* compute the sequence value */
    seq_t diff = S[K] - A_n_k;
    seq_t result = diff / 2;  /* division is exact */
    return result;
}
