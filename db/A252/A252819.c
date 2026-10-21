/*
 * A252819  Number of nX7 nonnegative integer arrays with upper left 0 and
 *          every value within 2 of its city block distance from the upper
 *          left and every value increasing by 0 or 1 with every step right or
 *          down.
 * https://oeis.org/A252819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is a polynomial of degree 14 in n; it is computed by
 *            Lagrange interpolation from the first 15 terms of the sequence.
 *            For all n ≥ 1, a(n) = Σ_{k=0}^{14} Δ^k a(1) * C(n-1, k), where
 *            Δ^k a(1) are the forward differences of the initial 15 terms.
 *            This gives the exact integer value for every n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 96, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const seq_t init[15] = {
    (seq_t)22,
    (seq_t)246,
    (seq_t)2067,
    (seq_t)13015,
    (seq_t)65711,
    (seq_t)279300,
    (seq_t)1033761,
    (seq_t)3414257,
    (seq_t)10248688,
    (seq_t)28359679,
    (seq_t)73160199,
    (seq_t)177550620,
    (seq_t)408377425,
    (seq_t)895696589,
    (seq_t)1883020893
};

static bool _coeff_ready = false;
static seq_t _coeff[15];  /* Δ^k a(1) for k=0..14 */

static void _make_coefficients(void)
{
    seq_t buf[15];
    for (idx_t i = 0; i < 15; i++)
        buf[i] = init[i];

    _coeff[0] = buf[0];
    for (idx_t k = 1; k <= 14; k++) {
        idx_t limit = 15 - k;
        for (idx_t i = 0; i < limit; i++)
            buf[i] = buf[i + 1] - buf[i];
        _coeff[k] = buf[0];
    }
    _coeff_ready = true;
}

/* binomial coefficient (n, k) where n and k are small (k <= 14) */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    seq_t r = (seq_t)1;
    for (idx_t i = 1; i <= k; i++)
        r = (r * (n - i + 1)) / i;
    return r;
}

seq_t a(idx_t n)
{
    if (!_coeff_ready)
        _make_coefficients();

    seq_t result = (seq_t)0;
    idx_t nn = n - 1;          /* we use C(n-1, k) */
    for (idx_t k = 0; k <= 14; k++)
        result += _coeff[k] * _binom(nn, k);

    return result;
}
