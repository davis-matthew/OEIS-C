/*
 * A291166  Connected Haar graph numbers.
 * https://oeis.org/A291166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A291166: returns the n‑th positive integer whose
   positions of 1's in the reversed binary expansion (bits indexed
   from the least significant bit of the original representation
   and counted starting at 1) have greatest common divisor 1.
   The function iterates integers in increasing order and stops
   when the n‑th qualifying integer is found. */

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b; 
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        idx_t g = 0;
        idx_t temp = m;
        idx_t j   = 0;
        while (temp) {
            if (temp & 1ULL) {
                idx_t pos = j + 1;          /* reversed‑position of this 1‑bit */
                g = (g == 0) ? pos : gcd_idx(g, pos);
                if (g == 1) break;          /* early exit */
            }
            temp >>= 1;
            ++j;
        }
        if (g == 1) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
