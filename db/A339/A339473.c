/*
 * A339473  Numbers k such that floor(sqrt(k)) divides k^2, but does not
 *          divide k.
 * https://oeis.org/A339473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A339473: Numbers k such that floor(sqrt(k))
   divides k² but does not divide k.
   Implementation: for each d = floor(sqrt(k)), compute s =
   ∏ p^{ceil(exp/2)} where d = ∏ p^{exp}.
   For every t = s, 2s, 3s, … up to 2d with t % d != 0,
   generate k = d*d + t and collect the terms in increasing order.
   Return the nth such k. */
seq_t a(idx_t n)
{
    idx_t count = 0;            /* number of terms found so far */
    for (idx_t d = 1;; ++d) {
        /* compute s = ∏ p^{ceil(exp/2)} for prime factorisation of d */
        idx_t temp = d;
        idx_t s = 1;
        for (idx_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                idx_t f = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++f;
                }
                idx_t e = (f + 1) / 2;      /* ceil(f/2) */
                idx_t pw = 1;
                for (idx_t i = 0; i < e; ++i) pw *= p;
                s *= pw;
            }
        }
        if (temp > 1) {                  /* remaining prime factor with exp = 1 */
            s *= temp;                   /* ceil(1/2) = 1 */
        }

        /* Enumerate valid t = s, 2s, 3s, … ≤ 2d, skipping multiples of d */
        for (idx_t t = s; t <= 2 * d; t += s) {
            if (t % d == 0) continue;    /* skip multiples of d itself */
            seq_t term = (seq_t)d * (seq_t)d + (seq_t)t;
            ++count;
            if (count == n) return term;
        }
    }
}
