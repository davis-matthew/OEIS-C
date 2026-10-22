/*
 * A349370  Dirichlet convolution of Kimberling's paraphrase, A003602, with
 *          itself.
 * https://oeis.org/A349370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} A003602(n/d) * A003602(d) where A003602(n) =
 *            (odd_part(n)+1)/2 (odd_part(n) = n divided by the largest power
 *            of 2 dividing n)
 * Range:     u8 n <= 104, u16 n <= 9404, u32 n <= 9016, u64 n <= 9016,
 *            u128 n <= 9016, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t f(idx_t n) {
    idx_t m = n;
    /* remove all factors of 2 */
    while ((m & 1) == 0) m >>= 1;
    /* (m+1)/2 for odd m = (m>>1) + 1, safe for all m */
    return ((seq_t)(m >> 1)) + 1;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;
            seq_t fd = f(d);
            seq_t fq = f(q);
            if (d == q)
                sum += fd * fq;           /* term when d == q */
            else
                sum += (fd * fq) * 2;     /* symmetric pair */
        }
    }
    return sum;
}
