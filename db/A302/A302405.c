/*
 * A302405  Total domination number of the n-prism graph.
 * https://oeis.org/A302405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) + 4, with the initial values a(0) = 0, a(1) = 1,
 *            a(2) = 2, a(3) = 2, a(4) = 4, a(5) = 4 (This recurrence holds
 *            for all n ≥ 6.)
 * Range:     u8 n <= 381, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t base[6] = {0, 1, 2, 2, 4, 4};
    idx_t q = n / 6;          /* number of full 6‑step cycles */
    idx_t r = n % 6;          /* position within the cycle   */
    return base[r] + 4 * (seq_t)q;
}
