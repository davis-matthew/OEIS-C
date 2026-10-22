/*
 * A391963  a(n) = Sum_{k=0..n} binomial(k+2,2) * binomial(k,2*(n-k)).
 * https://oeis.org/A391963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 30, u64 n <= 67, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 15*a(n-2) + 23*a(n-3) - 27*a(n-4)
                   + 24*a(n-5) - 16*a(n-6) + 9*a(n-7) - 3*a(n-8) + a(n-9)
   with initial values
   a(0)=1, a(1)=3, a(2)=6, a(3)=16, a(4)=45, a(5)=111,
   a(6)=253, a(7)=561, a(8)=1221. */

seq_t a(idx_t n) {
    /* Small‑n hard‑coded initial terms */
    if (n <= 8) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)3;
            case 2: return (seq_t)6;
            case 3: return (seq_t)16;
            case 4: return (seq_t)45;
            case 5: return (seq_t)111;
            case 6: return (seq_t)253;
            case 7: return (seq_t)561;
            case 8: return (seq_t)1221;
            default: /* unreachable */ return (seq_t)0;
        }
    }

    /* Store the last nine values in a circular buffer */
    seq_t a[9] = {
        (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)16,
        (seq_t)45, (seq_t)111, (seq_t)253,
        (seq_t)561, (seq_t)1221
    };

    for (idx_t i = 9; i <= n; ++i) {
        /* Indices of the nine preceding terms */
        seq_t prev1 = a[(i - 1) % 9];
        seq_t prev2 = a[(i - 2) % 9];
        seq_t prev3 = a[(i - 3) % 9];
        seq_t prev4 = a[(i - 4) % 9];
        seq_t prev5 = a[(i - 5) % 9];
        seq_t prev6 = a[(i - 6) % 9];
        seq_t prev7 = a[(i - 7) % 9];
        seq_t prev8 = a[(i - 8) % 9];
        seq_t prev9 = a[(i - 9) % 9];

        /* Positive contribution */
        seq_t pos = prev1 * 6 + prev3 * 23 + prev5 * 24 + prev7 * 9 + prev9;
        /* Negative contribution */
        seq_t neg = prev2 * 15 + prev4 * 27 + prev6 * 16 + prev8 * 3;

        a[i % 9] = pos - neg;              /* pos > neg for all n */
    }

    return a[n % 9];
}
