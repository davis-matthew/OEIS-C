/*
 * A268721  Convolution of A006068 (inverse of Gray code) with itself: a(n) =
 *          Sum_{k=1..n+1} A006068(k) * A006068(1+n-k).
 * https://oeis.org/A268721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n+1} A006068(k) * A006068(1+n-k)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 1023, u128 n <= 1023,
 *            big any n
 * Validated: standard, pass, 69 terms through n = 961 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t inv_gray(idx_t g)
{
    seq_t v = g;
    /* Apply successive XORs with right‑shifted copies until shift exceeds bit size.
       For arbitrary precision seq_t the shifts larger than the actual bit length do nothing. */
    for (idx_t shift = 1; shift < 64; shift <<= 1) {
        v ^= v >> shift;
    }
    return v;
}
/* where A006068(k) is the inverse Gray code of k (i.e. the unique n with n^(n>>1)=k) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Convolution of the inverse Gray code sequence with itself. */
    for (idx_t k = 1; k <= n + 1; ++k) {
        seq_t ak   = inv_gray(k);
        seq_t ak2  = inv_gray(n + 1 - k);
        sum += ak * ak2;
    }
    return sum;
}
