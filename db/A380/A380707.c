/*
 * A380707  a(n) = [x^n] Product_{k=0..n} (1 + (n^2+k)*x).
 * https://oeis.org/A380707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{k=0..n} (1 + (n² + k)·x)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented by the alternative binomial formula:        */
/* a(n) = (n+1)! × Σ_{k=0..n} C(n² + k – 1, k) / (n + 1 – k) */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t nsq = sn * sn;            /* n² */

    /* compute (n+1)! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n + 1; ++i)
        fact *= (seq_t)i;

    /* P = (n+1)! / (n+1) = n!    (k = 0 factor) */
    seq_t P = fact / (sn + (seq_t)1);

    /* combinatorial factor C(n² + k – 1, k), starting with k = 0 */
    seq_t comb = (seq_t)1;

    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = comb * P */
        seq_t term = comb * P;
        sum += term;

        if (k == n)
            break;   /* no further updates needed */

        /* update combinatorial for next k+1:
           C(n² + (k+1) – 1, k+1) = C(n² + k – 1, k) * (n² + k) / (k+1)      */
        comb = comb * (nsq + (seq_t)k) / ((seq_t)k + (seq_t)1);

        /* update P for next k+1:
           P_{k+1} = (n+1)! / (n + 1 – (k+1))
                  = P_k * (n + 1 – k) / (n – k)                          */
        P = P * ((sn + (seq_t)1) - (seq_t)k) / ((sn) - (seq_t)k);
    }

    return sum;
}
