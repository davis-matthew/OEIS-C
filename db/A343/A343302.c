/*
 * A343302  Numbers k such that k through k+4 are all deficient (in A005100).
 * https://oeis.org/A343302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 2320, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9239 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: A343302 – numbers k such that the five consecutive integers
   k, k+1, k+2, k+3, k+4 are all deficient (sigma(n) < 2n).  We test only
   candidates congruent to 1 modulo 6 and return the n‑th such k. */
#include <stdbool.h>
#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, safety for n=0 */

    idx_t count = 0;                /* how many qualifying k's seen */
    idx_t k = 1;                    /* 1 is 1 mod 6 and a valid start */

    for (;; k += 6) {               /* iterate over all k ≡ 1 (mod 6) */
        bool ok = true;
        for (idx_t off = 0; off < 5; ++off) {
            idx_t cur = k + off;
            seq_t sigma = sigma_of(cur);
            if (sigma >= (seq_t)cur * 2) {   /* not deficient */
                ok = false;
                break;
            }
        }
        if (ok) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}
