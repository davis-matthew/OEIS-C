/*
 * A380832  Number of points in Z^4 of norm <= n where the sum of the four
 *          entries is even.
 * https://oeis.org/A380832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 8 * (3 * Σ_{d=1}^{⌊n²/2⌋} d * ⌊⌊n²/2⌋/d⌋ - 2 *
 *            Σ_{e=1}^{⌊n²/4⌋} e * ⌊⌊n²/2⌋/(2e)⌋ ) - 32 * Σ_{d=1}^{⌊n²/4⌋} d *
 *            ⌊⌊n²/4⌋/d⌋ where all sums are over positive integers
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t mul128(seq_t a, seq_t b) {
    return (seq_t)((unsigned __int128)a * b);
}

/* Σ_{d=1}^{N} d * ⌊N/d⌋  */
static inline seq_t sum_floor_sigma(idx_t N) {
    seq_t res = 0;
    for (idx_t i = 1; i <= N; ) {
        idx_t q = N / i;
        idx_t j = N / q + 1;
        seq_t cnt = j - i;
        seq_t sum_i_j = (seq_t)i + (seq_t)(j - 1);
        seq_t s = mul128(sum_i_j, cnt) / 2;
        res += mul128((seq_t)q, s);
        i = j;
    }
    return res;
}

/* Σ_{e=1}^{⌊N/2⌋} e * ⌊N/(2e)⌋  */
static inline seq_t sum_floor_sigma_half(idx_t N) {
    seq_t res = 0;
    idx_t maxE = N / 2;
    for (idx_t e = 1; e <= maxE; ) {
        idx_t q = N / (2 * e);
        if (q == 0) break;
        idx_t e_next = N / (2 * q) + 1;
        if (e_next > maxE + 1) e_next = maxE + 1;
        seq_t cnt = e_next - e;
        seq_t sum_e = (seq_t)e + (seq_t)(e_next - 1);
        sum_e = mul128(sum_e, cnt) / 2;
        res += mul128((seq_t)q, sum_e);
        e = e_next;
    }
    return res;
}

seq_t a(idx_t n) {
    /* compute squares with 64‑bit intermediate then cast to idx_t */
    seq_t n_sq = mul128((seq_t)n, (seq_t)n);
    idx_t M = (idx_t)n_sq;
    idx_t A = M / 2;
    idx_t B = M / 4;

    seq_t sum1 = sum_floor_sigma(A);        /* Σ d*⌊A/d⌋ */
    seq_t sum2_half = sum_floor_sigma_half(A); /* Σ e*⌊A/(2e)⌋ */
    seq_t sum_sigma_2t = 3 * sum1 - 2 * sum2_half; /* Σ σ(2t) over t≤A */

    seq_t sum_sigma_B = sum_floor_sigma(B); /* Σ σ(m) over m≤B */

    seq_t result = 1 + 8 * sum_sigma_2t - 32 * sum_sigma_B;
    return result;
}
