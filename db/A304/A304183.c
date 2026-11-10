/*
 * A304183  Number of primitive inequivalent oblique sublattices of
 *          rectangular lattice of index n.
 * https://oeis.org/A304183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (A001615(n) - A304182(n))/2, with A001615 = Dedekind psi,
 *            A304182 multiplicative: f(2,1)=3, f(2,e>=2)=4, f(p odd,e)=2
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;

    seq_t psi = 1;
    seq_t mir = 1;
    idx_t t = n;

    for (idx_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                e++;
                t /= p;
            }
            seq_t c = (seq_t)p + 1;
            for (idx_t i = 1; i < e; i++) c *= (seq_t)p;
            psi *= c;
            if (p == 2) mir *= (seq_t)(e == 1 ? 3 : 4);
            else mir *= (seq_t)2;
        }
    }
    if (t > 1) {
        psi *= (seq_t)t + 1;
        if (t == 2) mir *= (seq_t)3;
        else mir *= (seq_t)2;
    }

    return (psi - mir) / 2;
}
