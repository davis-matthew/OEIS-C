/*
 * A253865  Number of (n+2) X (2+2) 0..1 arrays with every 3 X 3 subblock
 *          diagonal maximum minus antidiagonal minimum nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 38*a(n-2) - 82*a(n-3) + 1143*a(n-4) -
 *            3979*a(n-5) + 2864*a(n-6) - 15792*a(n-7) + 88960*a(n-8) -
 *            33152*a(n-9) - 191232*a(n-10) + 233216*a(n-11) - 294912*a(n-12)
 *            + 1916928*a(n-13) - 12189696*a(n-14) + 14680064*a(n-15) +
 *            12582912*a(n-16) - 16777216*a(n-17) Initial values: a(1)=332...
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base[18] = {
    0ULL,
    3328ULL,
    31632ULL,
    296960ULL,
    2589968ULL,
    22729728ULL,
    195774608ULL,
    1689797632ULL,
    14500025360ULL,
    124544611328ULL,
    1068223200144ULL,
    9166655759360ULL,
    78628228353808ULL,
    674532688614400ULL,
    5785876228976272ULL,
    49631400786527232ULL,
    425724993855617552ULL,
    3651819345018008576ULL
};

static const int coeffs[17] = {
    13, -38, -82, 1143, -3979, 2864, -15792, 88960,
    -33152, -191232, 233216, -294912, 1916928,
    -12189696, 14680064, 12582912, -16777216
};

static seq_t *cache = NULL;      /* cache[1..] */
static idx_t cache_cap = 0;      /* allocated capacity (n+1) */
static idx_t cache_max = 0;      /* highest n computed */

static void ensure_cache(idx_t n)
{
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n<1, but safe */

    /* Ensure cache can hold up to n */
    ensure_cache(n);

    /* Compute sequentially up to n if needed */
    if (cache_max < n) {
        /* Start from cache_max + 1 */
        idx_t start = cache_max + 1;
        if (start < 1) start = 1;
        for (idx_t i = start; i <= n; ++i) {
            if (i <= 17) {
                cache[i] = base[i];
            } else {
                seq_t pos = 0;
                seq_t neg = 0;
                /* i-1, i-2, ..., i-17 */
                for (int j = 0; j < 17; ++j) {
                    seq_t term = cache[i - 1 - j];
                    int cf = coeffs[j];
                    if (cf >= 0) {
                        pos += term * (seq_t)cf;
                    } else {
                        neg += term * (seq_t)(-cf);
                    }
                }
                cache[i] = pos - neg;  /* final positive result */
            }
        }
        cache_max = n;
    }

    return cache[n];
}
