/*
 * A270843  Number of nonisomorphic edge colorings of the Petersen graph with
 *          exactly n colors.
 * https://oeis.org/A270843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = \sum_{q=0}^n (-1)^q \binom{n}{q} A(n-q), where A(m) =
 *            (m^15 + 25*m^9 + 50*m^5 + 44*m^3) / 120 (numbers of
 *            edge‐colorings of the Petersen graph with at most m colors).
 *            This implements the inclusion–exclusion derived from the cycle
 *            index of the automorphism group. For n>15 the answer is 0, b...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   _pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_pow_seq.h"

static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > 15) return 0;

    static seq_t A[16] = {0};
    static bool ready = false;
    if (!ready) {
        /* Compute A[0..15] */
        A[0] = 0;
        for (idx_t m = 1; m <= 15; ++m) {
            seq_t m3 = _pow_seq(m, 3);
            seq_t m5 = _pow_seq(m, 5);
            seq_t m9 = _pow_seq(m, 9);
            seq_t m15 = _pow_seq(m, 15);
            seq_t numer = m15 + 25 * m9 + 50 * m5 + 44 * m3;
            A[m] = numer / 120;             /* integer division */
        }
        ready = true;
    }

    long long acc = 0;                       /* signed accumulator */
    for (idx_t q = 0; q <= n; ++q) {
        idx_t k = n - q;
        seq_t bin = _binom(n, q);
        seq_t term = bin * A[k];
        if (q & 1) acc -= (long long)term;
        else       acc += (long long)term;
    }
    return (seq_t)acc;
}
