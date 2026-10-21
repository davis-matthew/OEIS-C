/*
 * A278331  Shifted sequence of second differences of Genocchi numbers.
 * https://oeis.org/A278331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = G_{n+2} - 2*G_{n+3} + G_{n+4} where G_k are the Genocchi
 *            numbers, computed by the recurrence G_0 = 0, G_1 = 1 G_k = - (
 *            ∑_{i=0}^{k-1} C(k,i) G_i ) / 2 for k ≥ 2 Only the values up to
 *            n+4 are needed
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 15, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Static array and counter for memoised Genocchi numbers.
   The size 20005 covers all n up to 10000 (needs up to n+4). */
static seq_t G_vals[20005];
static idx_t G_len = 0;

/* Ensure that Genocchi numbers up to index 'needed' (inclusive) are computed. */
static void ensure_G(idx_t needed)
{
    if (needed < G_len) return;      /* already computed */

    /* initialise base values if necessary */
    if (G_len == 0) {
        G_vals[0] = (seq_t)0;
        G_vals[1] = (seq_t)1;
        G_len = 2;                    /* we have G_0 and G_1 ready */
    }

    for (idx_t k = G_len; k <= needed; ++k) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;          /* binomial(k,0) */

        for (idx_t i = 0; i < k; ++i) {
            if (i > 0) {
                /* bin = binomial(k,i) from binomial(k,i-1) */
                bin = bin * (seq_t)(k - i + 1) / (seq_t)i;
            }
            /* G_i is zero for odd i > 1, so we can skip them */
            if (i == 1 || (i % 2 == 0)) {
                sum += bin * G_vals[i];
            }
        }
        /* sum is always even; cast division is exact */
        G_vals[k] = - (sum / (seq_t)2);
    }
    G_len = needed + 1;
}

/* Public function computing the OEIS sequence A278331. */
seq_t a(idx_t n)
{
    /* We need Genocchi numbers up to index n+4 */
    ensure_G(n + 4);

    if ((n & (idx_t)1) == 0) {                /* n is even */
        return G_vals[n + 2] + G_vals[n + 4];
    } else {                                  /* n is odd */
        return - (seq_t)2 * G_vals[n + 3];
    }
}
