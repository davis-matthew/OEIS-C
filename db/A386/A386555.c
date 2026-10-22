/*
 * A386555  Number of 2-colorings of an 5 X 5 X 5 grid, up to rotational
 *          symmetry, by the number of black cells.
 * https://oeis.org/A386555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = [z^n] 1/24 ( (1+z)^125 * + 8 (1+z)^5 (1+z^3)^40 * + 9
 *            (1+z)^5 (1+z^2)^60 * + 6 (1+z)^5 (1+z^4)^30 ) * (cycle‑index for
 *            the rotational symmetry group of the cube)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 125,
 *            big any n
 * Helpers:   _compute_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/_compute_binom.h"

static bool _initialized = false;
static seq_t _coeff[126];

/* binomial coefficients, stored as 128‑bit to avoid overflow */
static unsigned __int128 _c125[126];
static unsigned __int128 _c5[6];
static unsigned __int128 _c40[41];
static unsigned __int128 _c60[61];
static unsigned __int128 _c30[31];

/* pre‑compute all polynomials once */
static void _compute_all(void)
{
    /* binomials */
    _compute_binom(_c125, 125);
    _compute_binom(_c5,   5);
    _compute_binom(_c40, 40);
    _compute_binom(_c60, 60);
    _compute_binom(_c30, 30);

    /* intermediate polynomials */
    static unsigned __int128 p2[126] = {0};
    static unsigned __int128 p3[126] = {0};
    static unsigned __int128 p4[126] = {0};

    /* (1+z)^5 * (1+z^3)^40 */
    for (idx_t i = 0; i <= 5; ++i) {
        unsigned __int128 ci = _c5[i];
        for (idx_t j = 0; j <= 40; ++j) {
            idx_t exp = i + 3 * j;
            if (exp <= 125)
                p2[exp] += ci * _c40[j];
        }
    }

    /* (1+z)^5 * (1+z^2)^60 */
    for (idx_t i = 0; i <= 5; ++i) {
        unsigned __int128 ci = _c5[i];
        for (idx_t j = 0; j <= 60; ++j) {
            idx_t exp = i + 2 * j;
            if (exp <= 125)
                p3[exp] += ci * _c60[j];
        }
    }

    /* (1+z)^5 * (1+z^4)^30 */
    for (idx_t i = 0; i <= 5; ++i) {
        unsigned __int128 ci = _c5[i];
        for (idx_t j = 0; j <= 30; ++j) {
            idx_t exp = i + 4 * j;
            if (exp <= 125)
                p4[exp] += ci * _c30[j];
        }
    }

    /* combine all terms, divide by 24 */
    for (idx_t n = 0; n <= 125; ++n) {
        unsigned __int128 val = _c125[n];
        val += 8 * p2[n];
        val += 9 * p3[n];
        val += 6 * p4[n];
        _coeff[n] = (seq_t)(val / 24);
    }

    _initialized = true;
}

seq_t a(idx_t n)
{
    if (n > 125) return 0;
    if (!_initialized) _compute_all();
    return _coeff[(size_t)n];
}
