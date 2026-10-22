/*
 * A371064  Array read by ascending antidiagonals where T(n,k) is the number
 *          of paths of length k from the origin to a facet of the cross
 *          polytope of size k in Z^n.
 * https://oeis.org/A371064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) is the value read by ascending antidiagonals of * T(n,k)
 *            where T(n,k) = Σ_{i=1..n} (-1)^(n-i) * binomial(n,i) * 2^i * i^k
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, pow2, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow_int.h"

/* Compute T(n,k) for n,k ≥ 1 */
static seq_t compute_T(idx_t n, idx_t k) {
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = binomial(n, i);
        term *= pow2(i);
        term *= pow_int((seq_t)i, k);
        if (((n - i) & 1U) == 0U)
            sum_pos += term;
        else
            sum_neg += term;
    }
    return sum_pos - sum_neg; /* non‑negative for n,k ≥ 1 */
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1 */
    /* Find diagonal s such that (s-1)*s/2 ≥ n */
    idx_t s = 2;
    idx_t total = 1; /* sum_{t=2..s} (t-1) for s=2 */
    while (total < n) {
        ++s;
        total += (s - 1);
    }
    idx_t prev_total = (s >= 2) ? ((s - 2) * (s - 1) / 2) : 0;
    idx_t pos   = n - prev_total; /* 1‑based position within diagonal s */
    idx_t row   = s - pos;         /* n coordinate (k≥1) */
    idx_t col   = pos;             /* k coordinate (n≥1) */
    return compute_T(row, col);
}
