/*
 * A344178  Difference between the arithmetic derivative of n and the
 *          cototient of n: a(n) = A003415(n) - A051953(n).
 * https://oeis.org/A344178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(n) - A051953(n) = phi(n) + n*( Σ(e_i/p_i) - 1 ) ,
 *            n = ∏ p_i^e_i
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the arithmetic derivative d(n) and the cototient n - φ(n).
   Then returns d(n) - (n - φ(n)) = φ(n) + d(n) - n. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {          /* offset 1 – undefined, return 0 */
        return 0;
    }

    uint64_t temp = n;
    seq_t n_seq = (seq_t)n;          /* convert index to seq_t */
    seq_t d = 0;                     /* arithmetic derivative */
    seq_t phi = n_seq;               /* Euler's totient */

    /* Trial division to factor n, computing d and phi simultaneously */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            /* d += e * (n / p) */
            d += (seq_t)e * (seq_t)(n / p);

            /* phi = phi / p * (p - 1) */
            phi /= (seq_t)p;
            phi *= (seq_t)(p - 1);
        }
    }

    /* Remaining prime factor */
    if (temp > 1) {
        uint64_t p = temp;
        uint64_t e = 1;
        d += (seq_t)e * (seq_t)(n / p);
        phi /= (seq_t)p;
        phi *= (seq_t)(p - 1);
    }

    /* a(n) = d + phi - n */
    seq_t a_val = d + phi;
    a_val -= n_seq;
    return a_val;
}
