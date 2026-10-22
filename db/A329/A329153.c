/*
 * A329153  Sum of the iterated unitary totient function (A047994).
 * https://oeis.org/A329153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1} uphi^k(n), where uphi(1)=1 and uphi(n) = n *
 *            Π_{p^e || n} (1 - 1/p^e). For this implementation uphi is
 *            computed via trial division on 64‑bit unsigned integers, which
 *            suffices for all n that fit in idx_t (uint64_t)
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _unitary_totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, if needed */
#include "../utils/_unitary_totient.h"

/* --- main sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* a(1) = 0 by definition */

    seq_t sum   = 0;
    idx_t cur   = n;

    while (1) {
        seq_t t = _unitary_totient(cur);
        sum += t;
        if (t == (seq_t)1) break;
        cur = (idx_t)t;           /* safe: t ≤ cur ≤ n ≤ 2^64−1 */
    }

    return sum;
}
