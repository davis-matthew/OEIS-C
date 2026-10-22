/*
 * A302652  Number of minimum total dominating sets in the n-antiprism graph.
 * https://oeis.org/A302652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Number of minimum total dominating sets in the
 *            n-antiprism graph Piecewise definition (k = floor((n - r)/7)
 *            with r = n mod 7): r = 0 : a(n) = 7 r = 1 : a(n) = 2*n r = 2 :
 *            a(n) = n*(32*k^2 + 38*k + 9)/3 r = 3 : a(n) = 4*n r = 4 : a(n) =
 *            n*(8*k + 6) r = 5 : a(n) = n*(8*k + 8)*(k + 2)*(4*k +...
 * Range:     u8 n <= 11, u16 n <= 32, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n < 1, return 0 for safety */
    idx_t r = n % 7;
    idx_t k = (n - r) / 7;
    seq_t N = (seq_t)n;
    seq_t kseq = (seq_t)k;
    seq_t res;

    switch (r) {
        case 0:  res = 7; break;
        case 1:  res = 2 * N; break;
        case 2: {
            seq_t val = 32 * kseq * kseq + 38 * kseq + 9;
            res = N * val / 3;
        } break;
        case 3:  res = 4 * N; break;
        case 4:  res = N * (8 * kseq + 6); break;
        case 5: {
            seq_t prod = N * (8 * kseq + 8) * (kseq + 2) * (4 * kseq + 3);
            res = prod / 3;
        } break;
        case 6:  res = 8 * N * (kseq + 1); break;
        default: res = 0; /* unreachable */
    }
    return res;
}
