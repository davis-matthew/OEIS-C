/*
 * A382800  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where A(n,k) = n! * k! * [x^n * y^k] 1 / (1 - log(1-x)
 *          * log(1-y))^3.
 * https://oeis.org/A382800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = sum_{j=0..min(n,k)} (j!)^2 * binomial(j+2,2) *
 *            unsigned_stirling_first_kind(n,j) *
 *            unsigned_stirling_first_kind(k,j) The sequence a(n) is obtained
 *            by reading the square array A(n,k) along antidiagonals
 *            downwards, starting with (0,0)
 * Range:     u8 n <= 22, u16 n <= 47, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define NMAX 200   /* sufficient for n up to about 10^7 (diagonals up to ~141) */

static bool _init = false;
static seq_t _fact[NMAX];
static seq_t _bin2[NMAX];
static seq_t *_stirling = NULL;   /* array of size NMAX*NMAX */

/* initialization of factorials, binomial(j+2,2), and unsigned Stirling numbers of
   the first kind (signless) */
static void _init_tables(void)
{
    _stirling = (seq_t *)malloc(NMAX * NMAX * sizeof(seq_t));
    for (idx_t i = 0; i < NMAX; ++i)
        for (idx_t j = 0; j < NMAX; ++j)
            _stirling[i * NMAX + j] = 0;

    _fact[0] = 1;
    _bin2[0] = 1;
    for (idx_t n = 1; n < NMAX; ++n) {
        _fact[n] = _fact[n - 1] * (seq_t)n;
        _bin2[n] = ((seq_t)(n + 2) * (n + 1)) / 2;
    }

    _stirling[0 * NMAX + 0] = 1;
    for (idx_t n = 1; n < NMAX; ++n) {
        for (idx_t k = 1; k <= n; ++k) {
            seq_t a = _stirling[(n - 1) * NMAX + (k - 1)];
            seq_t b = _stirling[(n - 1) * NMAX + k];
            seq_t mul = b * (seq_t)(n - 1);
            _stirling[n * NMAX + k] = a + mul;
        }
    }
}

seq_t a(idx_t n)
{
    if (!_init) {
        _init_tables();
        _init = true;
    }

    /* find diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    for (;;) {
        idx_t tri = s * (s + 1) / 2;
        if (n < tri) {
            --s;
            break;
        }
        ++s;
    }

    idx_t offset = n - s * (s + 1) / 2;
    idx_t i = offset;        /* row index */
    idx_t k = s - offset;    /* column index */

    if (k > i) {             /* use symmetry */
        idx_t t = i;
        i = k;
        k = t;
    }

    idx_t m = (i < k) ? i : k;
    seq_t res = 0;

    for (idx_t j = 0; j <= m; ++j) {
        seq_t term = _fact[j];
        term = term * _fact[j];
        term = term * _bin2[j];
        term = term * _stirling[i * NMAX + j];
        term = term * _stirling[k * NMAX + j];
        res = res + term;
    }

    return res;
}
