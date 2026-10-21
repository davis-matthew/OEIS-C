/*
 * A258096  Expansion of psi(x^4) * phi(-x^4)^4 / phi(-x) in powers of x where
 *          phi(), psi() are Ramanujan theta function.
 * https://oeis.org/A258096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Euler transform of the period‑8 sequence c = [2, 1, 2, -6, 2, 1,
 *            2, -4], i.e. a(0) = 1, a(n) = (1/n) * Σ_{k=1..n} ( Σ_{d|k} d *
 *            c[d] ) * a(n-k) (for n ≥ 1). This generates the sequence A258096
 * Range:     u8 n <= 107, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required types: seq_t, idx_t */

#include <stdlib.h>
#include <stdint.h>

static seq_t *b = NULL;             /* cached values a(0)…a(max_n) */
static int64_t *sint = NULL;        /* cached s(k) = Σ_{d|k} d*c[d] */
static idx_t max_n_cached = 0;      /* highest n whose values are cached */

/* c[d] for d = 1..8 (period 8 sequence) */
static const int c_arr[9] = {0, 2, 1, 2, -6, 2, 1, 2, -4};

/* Ensure that arrays b[0..n] and sint[1..n] are computed and stored. */
static void ensure_cache(idx_t n)
{
    if (b != NULL && n <= max_n_cached)
        return;                      /* already computed */

    /* Allocate new size (n+1) for b and sint */
    size_t new_cap = (size_t)n + 1;
    seq_t *new_b = NULL;
    int64_t *new_sint = NULL;

    if (b == NULL)
        new_b = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        new_b = (seq_t *)realloc(b, new_cap * sizeof(seq_t));

    if (sint == NULL)
        new_sint = (int64_t *)malloc(new_cap * sizeof(int64_t));
    else
        new_sint = (int64_t *)realloc(sint, new_cap * sizeof(int64_t));

    if (new_b == NULL || new_sint == NULL) {
        abort();
    }

    b = new_b;
    sint = new_sint;

    /* Fill missing s(k) values */
    for (idx_t k = max_n_cached ? max_n_cached + 1 : 1; k <= n; ++k) {
        int64_t sum = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                int idx = (int)((d - 1) % 8) + 1;
                sum += (int64_t)d * (int64_t)c_arr[idx];
                idx_t e = k / d;
                if (e != d) {
                    int idx2 = (int)((e - 1) % 8) + 1;
                    sum += (int64_t)e * (int64_t)c_arr[idx2];
                }
            }
        }
        sint[k] = sum;
    }

    /* If this is the first time, set b[0] = 1 */
    if (max_n_cached == 0 && b[0] == 0)
        b[0] = (seq_t)1;

    /* Compute a(k) for new indices */
    for (idx_t i = max_n_cached ? max_n_cached + 1 : 1; i <= n; ++i) {
        __int128 total = 0;
        for (idx_t k = 1; k <= i; ++k) {
            __int128 prod = (__int128)sint[k] * (__int128)b[i - k];
            total += prod;
        }
        /* integer division: total is guaranteed divisible by i */
        b[i] = (seq_t)(total / (__int128)i);
    }

    max_n_cached = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* base case */

    ensure_cache(n);
    return b[n];
}
