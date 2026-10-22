/*
 * A304366  Numbers with additive persistence = 1.
 * https://oeis.org/A304366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer ≥10 whose decimal digit sum ≤ 9 (i.e. numbers
 *            with additive persistence = 1)
 * Range:     u8 n <= 123, u16 n <= 1971, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Binomial coefficient C(n,k) for small n,k (≤ ~15). */
static seq_t binomial_small(uint64_t n, uint64_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (uint64_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n = 0 */
    seq_t n_seq = (seq_t)n;

    /* Find the digit‑length k such that the cumulative count reaches n. */
    seq_t cum = (seq_t)0;
    uint64_t k = 2;
    seq_t count_k;
    for (;;) {
        count_k = binomial_small(8 + k, k);          /* count of k‑digit numbers with sum≤9 */
        if (cum + count_k >= n_seq) break;
        cum += count_k;
        ++k;
    }

    seq_t remaining = n_seq - cum;
    seq_t result = (seq_t)0;
    uint64_t sum_sofar = 0;

    /* Construct the number digit by digit. */
    for (uint64_t pos = 1; pos <= k; ++pos) {
        uint64_t start_d = (pos == 1) ? 1 : 0;
        uint64_t r = k - pos;                 /* positions left after this one */

        for (uint64_t d = start_d; d <= 9; ++d) {
            if (sum_sofar + d > 9) continue;   /* violates the sum constraint */

            uint64_t rem_sum = 9 - (sum_sofar + d);
            seq_t count = binomial_small(rem_sum + r, r);

            if (remaining > count) {
                remaining -= count;            /* skip this block */
            } else {
                result = result * 10 + d;       /* fix this digit */
                sum_sofar += d;
                break;
            }
        }
    }

    return result;
}
