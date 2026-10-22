/*
 * A365099  Number of distinct residues of x^n (mod n^2), x=0..n^2-1.
 * https://oeis.org/A365099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p^e‖n} ( U(p,e) + 1 ) where * U(p,e) is the number of
 *            distinct n‑th power residues * in the unit group modulo p^{2e}:
 *            * • if p is odd U = φ(p^{2e}) / gcd( n , φ(p^{2e}) ) * • if p =
 *            2: * let m = 2^{2e‑2} ( = (p^{2e})/4 ), * let φ = 2^{2e‑1}, * if
 *            n is even U = (φ/2) / gcd( n , m ) * if...
 * Range:     u8 n <= 18, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External utilities provided by the build system */
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;            /* a(1) = 1 */

    idx_t temp = n;                  /* remaining part to factorise */
    seq_t result = (seq_t)1;          /* product over prime powers */

    /* Trial division: handle 2 separately, then only odds */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p != 0)
            continue;

        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        idx_t k = 2 * e;             /* exponent in modulus p^{2e} */

        seq_t U;                      /* number of unit n‑th‑power residues */

        if (p == 2) {
            /* φ = 2^{k-1},  m = 2^{k-2} */
            seq_t phi = (seq_t)1 << (k - 1);
            seq_t m   = (seq_t)1 << (k - 2);
            seq_t g   = gcd((seq_t)n, m);

            if (n & 1U)              /* n odd */
                U = phi / g;
            else                     /* n even */
                U = (phi >> 1) / g;  /* φ/2 */
        } else {
            /* φ = p^{k-1} * (p-1) */
            seq_t p_pow = seq_pow((seq_t)p, k - 1);  /* p^{k-1} */
            seq_t phi   = p_pow * (seq_t)(p - 1);
            seq_t g     = gcd((seq_t)n, phi);
            U = phi / g;
        }

        result *= (U + (seq_t)1);
    }

    /* If a prime greater than sqrt(n) remains */
    if (temp > 1) {
        idx_t p = temp;          /* remaining prime factor, exponent e = 1 */
        idx_t k = 2;              /* 2*e = 2 */

        seq_t U;
        if (p == 2) {
            seq_t phi = (seq_t)1 << 1;   /* φ(4) = 2 */
            seq_t m   = (seq_t)1 << 0;   /* 2^{0} = 1 */
            seq_t g   = gcd((seq_t)n, m);
            U = (n & 1U) ? (phi / g) : ((phi >> 1) / g);
        } else {
            seq_t phi = (seq_t)(p - 1) * p;   /* p * (p-1) = φ(p²) */
            seq_t g   = gcd((seq_t)n, phi);
            U = phi / g;
        }
        result *= (U + (seq_t)1);
    }

    return result;
}
