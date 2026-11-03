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
 * Validated: standard, pass, 81 terms through n = 9638 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, if needed */

/* --- unitary totient for 64‑bit values ----------------------------------- */
static seq_t _unitary_totient(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t result = n;
    idx_t temp   = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t p_pow = 1;
            while (temp % p == 0) {
                temp  /= p;
                p_pow *= p;
            }
            /* result = (result / p_pow) * (p_pow - 1)  */
            result = (result / p_pow) * (p_pow - 1);
        }
    }

    if (temp > 1) {
        /* remaining prime factor > sqrt(original n) */
        result = (result / temp) * (temp - 1);
    }

    return (seq_t)result;
}

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
