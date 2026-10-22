/*
 * A380753  E.g.f. A(x) satisfies A(x) = exp(x * A(x)^2) / (1 - x * A(x)^2)^2.
 * https://oeis.org/A380753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ(k=0..n) (2n+1)^(k-1) * binomial(5n‑k+1, n‑k) / k!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial coefficient: binom(N, K) == N! / (K! (N-K)!) */
static seq_t binom(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;          /* use symmetry to reduce iterations */
    seq_t result = 1;
    for (idx_t i = 1; i <= K; ++i) {
        result *= (seq_t)(N - K + i);
        result /= (seq_t)i;           /* exact division */
    }
    return result;
}

/* Returns the n-th term of sequence A380753 (offset 0) */
seq_t a(idx_t n)
{
    /* 2n+1 */
    seq_t base = (seq_t)(2 * n + 1);

    /* factorial n! */
    seq_t fact_n = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact_n *= (seq_t)i;
    }

    /* Initial binomial: binom(5n+1, n) */
    seq_t B = binom(5 * n + 1, n);

    /* Sum accumulator */
    seq_t sum = 0;

    /* k = 0 term: n! * B / base */
    sum += (fact_n * B) / base;

    /* Prepare for k = 1 .. n */
    seq_t fact_div = fact_n;          /* n! / k!  (starts with k!=0!) */
    seq_t pow_base = 1;              /* base^(k-1), starts with k=1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update factorial division: n! / k! */
        fact_div /= (seq_t)k;

        /* Update binomial using recurrence:
           B_k = B_{k-1} * (n - k + 1) / (5n - k + 2) */
        B *= (seq_t)(n - k + 1);
        B /= (seq_t)(5 * n - k + 2);

        /* Term for this k: (n!/k!) * B * base^(k-1) */
        seq_t term = fact_div * B * pow_base;
        sum += term;

        /* Prepare pow_base for next iteration: multiply by base */
        pow_base *= base;
    }

    return sum;
}
