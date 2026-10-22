/*
 * A355407  Expansion of the e.g.f. log((1 - x) / (1 - 2*x)) / (1 - x)^4.
 * https://oeis.org/A355407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * sum_{k=1..n} binom(n - k + 3, 3) * (2^k - 1)/k,
 *            computed via a recurrence derived from the egf
 *            log((1-x)/(1-2x))/(1-x)^4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t a_prev[4] = {0, 0, 0, 0};   /* a[n-1], a[n-2], a[n-3], a[n-4] for current n */
    seq_t fact = 1;                  /* (n-1)! before loop */
    seq_t pow2 = 1;                  /* 2^0 */

    for (idx_t cur = 1; cur <= n; ++cur) {
        pow2 *= (seq_t)2;            /* 2^cur */
        seq_t prev_fact = fact;      /* (cur-1)! */
        fact *= (seq_t)cur;          /* cur! */

        /* term1 = (cur-1)! * (2^cur - 1) */
        seq_t term1 = prev_fact * ((pow2) - (seq_t)1);

        /* ratios for the recurrence */
        seq_t ratio1 = (seq_t)cur;
        seq_t ratio2 = ratio1 * ((seq_t)cur - 1);
        seq_t ratio3 = ratio2 * ((seq_t)cur - 2);
        seq_t ratio4 = ratio3 * ((seq_t)cur - 3);

        seq_t temp = term1;
        /* +4 * ratio1 * a[n-1] (since -(-4)*(...) = +4*(...)) */
        temp += (seq_t)4 * ratio1 * a_prev[0];
        if (cur >= 2) temp -= (seq_t)6 * ratio2 * a_prev[1];
        if (cur >= 3) temp += (seq_t)4 * ratio3 * a_prev[2];
        if (cur >= 4) temp -= (seq_t)1 * ratio4 * a_prev[3];

        /* shift previous values for next iteration */
        a_prev[3] = a_prev[2];
        a_prev[2] = a_prev[1];
        a_prev[1] = a_prev[0];
        a_prev[0] = temp;
    }

    return a_prev[0];   /* a(n) */
}
