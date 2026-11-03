/*
 * A331153  Triangle read by rows: T(n,k) (n>=k>=1) = round((n/k)*round(n/k)).
 * https://oeis.org/A331153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = round((n/k)*round(n/k)) (triangle read by rows)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t round_div_idx(idx_t a, idx_t b) {
    idx_t q = a / b;
    idx_t r = a % b;
    if (r * 2 >= b) q++;
    return q;
}

static inline seq_t round_div_seq(seq_t a, seq_t b) {
    seq_t q = a / b;
    seq_t r = a % b;
    if (r * 2 >= b) q++;
    return q;
}

seq_t a(idx_t N) {
    idx_t total = 0;
    for (idx_t row = 1; ; row++) {
        for (idx_t k = 1; k <= row; k++) {
            /* compute round(n/k) for integer n=row, k */
            idx_t r1 = round_div_idx(row, k);
            /* compute round((n/k)*r1) = round(n*r1/k) */
            seq_t num = (seq_t)row * (seq_t)r1;
            seq_t term = round_div_seq(num, (seq_t)k);
            total++;
            if (total == N) return term;
        }
    }
}
