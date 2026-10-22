/*
 * A309491  Let gcd_2(b,c) be the second-largest common divisor of non-coprime
 *          integers b and c; then a(n) = Sum_{k=1..n} gcd_2(k,n). If b and c
 *          are coprime, then gcd_2(b,c) = 0.
 * https://oeis.org/A309491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} gcd_2(k,n) where gcd_2(b,c) is the
 *            second-largest common divisor of non‑coprime integers b and c,
 *            and is 0 if gcd(b,c)=1
 * Range:     u8 n <= 95, u16 n <= 6719, u32 n <= 9770, u64 n <= 9770,
 *            u128 n <= 9770, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Return the smallest prime factor of g (or g itself if g is prime). */
static seq_t small_prime_factor(seq_t g) {
    for (idx_t d = 2; (seq_t)d * (seq_t)d <= g; ++d) {
        if (g % d == 0) return d;
    }
    return g;
}

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t g = gcd((seq_t)k, (seq_t)n);
        if (g > 1) {
            seq_t spf = small_prime_factor(g);   /* smallest prime factor */
            total += g / spf;                    /* second‑largest divisor */
        }
    }
    return total;
}
