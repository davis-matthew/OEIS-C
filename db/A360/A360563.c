/*
 * A360563  Number of ordered multisets of size n with elements from [n] whose
 *          element sum is larger than the product of all elements.
 * https://oeis.org/A360563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered n‑tuples (a1 … an) with ai∈[1,n] for
 *            which Σai > ∏ai. The routine enumerates all distinct multisets
 *            of numbers ≥ 2 whose product does not exceed 14 n, computes the
 *            multiplicity factor, checks the inequality, and multiplies by
 *            the binomial coefficient that counts the place...
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 89, u64 n <= 150, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system. */

static idx_t  _g_n;
static uint32_t _g_max_depth;
static uint64_t _g_limit;
static seq_t  _g_fact[32];
static seq_t  _g_binom[32];
static uint32_t _g_values[32];
static seq_t  _g_ans;

/* Recursive generation of non‑decreasing multisets of values ≥2 */
static void _enumerate(uint32_t start, uint64_t prod,
                       uint32_t sum, uint8_t depth)
{
    if (depth > 0) {
        uint32_t k = depth;
        uint64_t t = prod - sum + k;
        if (t < _g_n) {
            /* multiplicity factor k! / Π(count!) */
            seq_t coeff = _g_fact[k];
            uint8_t cnt = 1;
            for (uint8_t i = 1; i < k; ++i) {
                if (_g_values[i] == _g_values[i-1]) {
                    ++cnt;
                } else {
                    coeff /= _g_fact[cnt];
                    cnt = 1;
                }
            }
            coeff /= _g_fact[cnt];

            seq_t add = coeff * _g_binom[k];
            _g_ans += add;
        }
    }

    if (depth == _g_max_depth) return;

    uint32_t vmax = (uint32_t)_g_n;
    uint32_t vlim = (uint32_t)(_g_limit / prod);
    if (vlim > vmax) vlim = vmax;

    for (uint32_t v = start; v <= vlim; ++v) {
        _g_values[depth] = v;
        _enumerate(v, prod * (uint64_t)v, sum + v, depth + 1);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;        /* a(0)=0 , a(1)=0 */

    _g_n = n;
    _g_limit = 14ULL * n;

    /* maximum multiset size that can occur (product ≤ 14 n) */
    uint32_t max_d = 0;
    uint64_t pow2 = 1;
    while (pow2 <= _g_limit && max_d < 30) {    /* safety cap */
        ++max_d;
        pow2 <<= 1;
    }
    _g_max_depth = max_d;

    /* factorials up to max_d */
    _g_fact[0] = (seq_t)1;
    for (uint32_t i = 1; i <= max_d; ++i)
        _g_fact[i] = _g_fact[i-1] * (seq_t)i;

    /* binomial coefficients C(n,k) for 0 ≤ k ≤ max_d */
    _g_binom[0] = (seq_t)1;
    for (uint32_t k = 1; k <= max_d; ++k) {
        if (k <= n) {
            seq_t numer = _g_binom[k-1] * (seq_t)(n - k + 1);
            seq_t denom = (seq_t)k;
            _g_binom[k] = numer / denom;
        } else {
            _g_binom[k] = (seq_t)0;
        }
    }

    _g_ans = (seq_t)1;                /* all‑ones multiset */
    _enumerate(2, 1ULL, 0U, 0);

    return _g_ans;
}
