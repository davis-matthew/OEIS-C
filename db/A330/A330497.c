/*
 * A330497  a(n) = n! * Sum_{k=0..n} (-1)^k * binomial(n,k) * n^(n - k) / k!.
 * https://oeis.org/A330497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n,k)^2 * n^k * k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply two seq_t values, using a 128‑bit intermediate when possible. */
static inline seq_t mul_seq(seq_t a, seq_t b)
{
#if defined(__cplusplus)
    /* C++ build – use the built‑in operator, which works for arbitrary‑precision types. */
    return a * b;
#else
    #if defined(__SIZEOF_INT128__)
        return (seq_t)((unsigned __int128)a * (unsigned __int128)b);
    #else
        return a * b;
    #endif
#endif
}

seq_t a(idx_t n)
{
    /* The special case n == 0 gives the required first term. */
    if (n == 0) return (seq_t)1;

    seq_t binom   = (seq_t)1;  /* C(n,0) */
    seq_t pow_n   = (seq_t)1;  /* n^0   */
    seq_t fact_k  = (seq_t)1;  /* 0!    */

    seq_t sum_pos = (seq_t)0;
    seq_t sum_neg = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        /* term = binom^2 * n^k * k! */
        seq_t term = mul_seq(binom, binom);
        term = mul_seq(term, pow_n);
        term = mul_seq(term, fact_k);

        /* Add with the appropriate sign. */
        if (((n - k) & 1U) == 0U)
            sum_pos = sum_pos + term;
        else
            sum_neg = sum_neg + term;

        if (k == n) break;  /* Finished the last term. */

        /* Update binomial coefficient: C(n,k+1) = C(n,k) * (n-k) / (k+1) */
        binom = mul_seq(binom, (seq_t)(n - k));
        binom = binom / (seq_t)(k + 1);

        pow_n   = mul_seq(pow_n, (seq_t)n);
        fact_k  = mul_seq(fact_k, (seq_t)(k + 1));
    }

    /* The partial sums satisfy sum_pos >= sum_neg, so unsigned subtraction is safe. */
    return sum_pos - sum_neg;
}
