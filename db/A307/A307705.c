/*
 * A307705  Expansion of Product_{k>=1} 1/(1 - x^k)^(k-phi(k)), where phi() is
 *          the Euler totient function (A000010).
 * https://oeis.org/A307705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} 1/(1 - x^k)^(k-φ(k)) where
 *            φ is Euler's totient. The logarithmic derivative gives the
 *            recurrence n * a(n) = Σ_{k=1..n} a(n-k) * B(k), with B(k) =
 *            Σ_{d|k} (d^2 - d·φ(d)). a(0) = 1
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* cached data, extended lazily on demand */
static idx_t _a_max_n = 0;      /* largest n already computed        */
static seq_t *_a_vals       = NULL; /* a[0.._a_max_n]                     */
static seq_t *_phi_vals    = NULL; /* φ[0.._a_max_n] (φ[0] unused)       */
static seq_t *_B_vals      = NULL; /* B[0.._a_max_n]  (B[0] unused)      */

static void _ensure_cache(idx_t n)
{
    if (n <= _a_max_n)
        return;

    /* Resize caches if needed */
    if (_a_max_n == 0) {
        _a_vals  = (seq_t*)calloc(n + 1, sizeof(seq_t));
        _phi_vals= (seq_t*)calloc(n + 1, sizeof(seq_t));
        _B_vals  = (seq_t*)calloc(n + 1, sizeof(seq_t));
    } else {
        _a_vals  = (seq_t*)realloc(_a_vals,  (n + 1) * sizeof(seq_t));
        _phi_vals= (seq_t*)realloc(_phi_vals,(n + 1) * sizeof(seq_t));
        _B_vals  = (seq_t*)realloc(_B_vals,  (n + 1) * sizeof(seq_t));
    }

    /* -------------   compute φ[1..n]   ------------- */
    for (idx_t i = 0; i <= n; ++i)
        _phi_vals[i] = (seq_t)0;
    _phi_vals[1] = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        _phi_vals[i] = (seq_t)i;           /* init with n (prime test later) */

    for (idx_t i = 2; i <= n; ++i) {
        if (_phi_vals[i] == (seq_t)i) {      /* i is prime   */
            for (idx_t j = i; j <= n; j += i)
                _phi_vals[j] -= _phi_vals[j] / (seq_t)i;
        }
    }

    /* -------------   compute B[1..n]   ------------- */
    for (idx_t i = 0; i <= n; ++i)
        _B_vals[i] = (seq_t)0;               /* zero init */

    for (idx_t d = 1; d <= n; ++d) {
        /* e_d = d^2 - d·φ(d)  (all values fit in seq_t) */
        seq_t d_seq = (seq_t)d;
        seq_t e_d = d_seq * d_seq - d_seq * _phi_vals[d];
        for (idx_t m = d; m <= n; m += d)
            _B_vals[m] += e_d;
    }

    /* -------------   compute a[0..n]   ------------- */
    if (_a_max_n == 0)
        _a_vals[0] = (seq_t)1;              /* a(0) = 1 */

    for (idx_t m = _a_max_n + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k)
            sum += _a_vals[m - k] * _B_vals[k];
        _a_vals[m] = sum / (seq_t)m;        /* integer division is exact */
    }

    _a_max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1, already defined */
        return (seq_t)1;
    _ensure_cache(n);
    return _a_vals[n];
}
