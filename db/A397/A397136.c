/*
 * A397136  Dirichlet g.f.: Product_{p prime} (1 + 2/p^s + 1/p^(2*s-1)).
 * https://oeis.org/A397136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} f(p,e) with * f(p,1)=2, f(p,2)=p, f(p,e)=0 for
 *            e≥3. * Multiplicativity allows simple prime‑factorisation
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* multiplicative function a(n) for OEIS A397136 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, behaviour for 0 irrelevant */
    if (n == 1) return 1;          /* a(1)=1 */

    idx_t m   = n;                 /* remaining part to factor */
    seq_t res = 1;

    /* handle factor 2 separately for speed */
    unsigned exp = 0;
    while ((m & 1ULL) == 0) {      /* while m divisible by 2 */
        m >>= 1;
        ++exp;
        if (exp >= 3) return 0;
    }
    if (exp == 1) res = (seq_t)2;          /* a(2)=2   */
    else if (exp == 2) res = (seq_t)2;     /* a(4)=2   */

    /* trial division for odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p != 0) continue;
        exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
            if (exp >= 3) return 0;
        }
        if (exp == 1) {
            res = res * (seq_t)2;
        } else { /* exp == 2 */
            res = res * (seq_t)p;
        }
    }

    /* if a prime factor > sqrt(original n) remains */
    if (m > 1) {
        res = res * (seq_t)2;
    }

    return res;
}
