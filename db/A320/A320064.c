/*
 * A320064  The number of F_2 graphs on { 1, 2, ..., n } with a unique cycle
 *          of weight 1, which corresponds to the number of reflectable bases
 *          of the root system of type D_n.
 * https://oeis.org/A320064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-2) * A000435(n), A000435(n) = (n-1)! *
 *            Sum_{k=0..n-2} n^k/k!; a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 330 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A000435_a(idx_t n) {
    if (n == 1) return 0;

    seq_t result = 0;
    seq_t n_seq = (seq_t)n;

    seq_t term_factor = 1;
    for (idx_t i = 1; i < n; i++) {
        term_factor *= (seq_t)i;
    }

    seq_t n_pow = 1;

    for (idx_t k = 0; k + 2 <= n; k++) {
        result += term_factor * n_pow;
        n_pow *= n_seq;
        term_factor /= (seq_t)(k + 1);
    }

    return result;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    seq_t r = A000435_a(n);
    for (idx_t i = 0; i + 2 < n; i++) {
        r *= 2;
    }
    return r;
}
