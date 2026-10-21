/*
 * A248527  Numbers k such that the smallest prime divisor of k^2+1 is 13.
 * https://oeis.org/A248527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = base[(n-1)%6] + 130*((n-1)/6) * where base =
 *            {34,44,60,70,86,96} (the first six terms)
 * Range:     u8 n <= 12, u16 n <= 3024, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t base[6] = { (seq_t)34, (seq_t)44, (seq_t)60, (seq_t)70, (seq_t)86, (seq_t)96 };
seq_t a(idx_t n) {
    idx_t idx   = n - 1;       /* zero‑based index */
    idx_t block = idx / 6;     /* how many full 6‑term blocks have passed */
    idx_t off   = idx % 6;     /* position inside the current block   */
    seq_t step  = (seq_t)block * (seq_t)130; /* 130 per block         */
    return step + base[off];
}
