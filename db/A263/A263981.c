/*
 * A263981  Least even k such that phi(k) >= n.
 * https://oeis.org/A263981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least even k such that φ(k) ≥ n, where φ is Euler’s
 *            totient. The sequence is defined for n ≥ 1 (offset 1). This
 *            implementation searches k ≥ 2*n incrementing by 2 and evaluates
 *            φ(k) by a simple trial‑division factorisation. The search time
 *            is O(√k) per candidate, which is easily fast enough...
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ---- local phi calculation ------------------------------------------------ */
static seq_t phi_calc(seq_t m)
{
    /* φ(1) = 1 */
    if (m == 1) return (seq_t)1;

    seq_t result = m;                 /* m */
    seq_t p = 2;                      /* current prime factor */

    while (p * p <= m) {
        if (m % p == 0) {
            result = result / p * (p - 1);
            while (m % p == 0) m /= p;
        }
        ++p;
    }
    if (m > 1) {                      /* remaining prime factor */
        result = result / m * (m - 1);
    }
    return result;
}

/* ---- OEIS sequence generator ---------------------------------------------- */
seq_t a(idx_t n)
{
    /* start from the obvious lower bound 2*n (always even) */
    seq_t k = (seq_t)n << 1;          /* 2*n */

    /* ensure k is at least 2 */
    if (k < 2) k = 2;

    /* increment by two until we find φ(k) ≥ n */
    while (phi_calc(k) < (seq_t)n)
        k += 2;

    return k;
}
