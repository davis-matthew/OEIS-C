/*
 * A337332  a(n) =
 *          Sum_{k=0..n}C(n,k)*C(n+k,k)*C(2k,k)*C(2n-2k,n-k)*(-8)^(n-k).
 * https://oeis.org/A337332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) =
 *            Sum_{k=0..n}C(n,k)*C(n+k,k)*C(2k,k)*C(2n-2k,n-k)*(-8)^(n-k)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 14, i128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A337332. */

#include <stddef.h>
#include "../utils/binomial.h" /* provides seq_t binomial(idx_t, idx_t) */

/* Helper: multiply two seq_t values safely, using 128‑bit intermediate
   when available and seq_t is 64-bit or smaller. */
static inline seq_t mul_big(seq_t a, seq_t b)
{
#ifdef __SIZEOF_INT128__
    if (sizeof(seq_t) <= 8) /* 8 bytes or less (int64_t or smaller) */
        return (seq_t)((__int128)a * b);
#endif
    return a * b;
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pwr = 1;                /* (-8)^(n-k), start with k = n */

    for (idx_t k = n; ; --k) {
        /* Binomial terms */
        seq_t Cnk = binomial(n, k);
        seq_t An   = binomial(n + k, k);
        seq_t Bk   = binomial(2 * k, k);
        seq_t Ck   = binomial(2 * n - 2 * k, n - k);

        /* Product of binomials */
        seq_t term = mul_big(Cnk,
                          mul_big(An,
                              mul_big(Bk, Ck)));

        /* Add weighted term to sum */
        sum = sum + mul_big(term, pwr);

        /* Check loop termination */
        if (k == 0) break;

        /* Update power for next iteration (k-1) */
        pwr = mul_big(pwr, (seq_t)(-8));
    }

    return sum;
}
