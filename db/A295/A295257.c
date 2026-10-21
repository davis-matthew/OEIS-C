/*
 * A295257  Expansion of e.g.f. cot(x)*(1 - sqrt(1 - 4*tan(x)))/2.
 * https://oeis.org/A295257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A295257
    a(n) = coefficient of x^n/n! in C(tan(x))  where
    C(z) = 1 + z*C(z)^2  (Catalan generating function).
    The coefficients g[n] used below are the tangent
    numbers: g[1] = 1, g[2] = 0, g[3] = 2, g[4] = 0, g[5] = 16, … .
    The recurrence for the EGF H(x) = a(x) is
        H(0) = 1,
        H[n] = Σ_{i=1..n} binom(n,i) * g[i] * D[n-i],   n≥1
    where D[m] = Σ_{j=0..m} binom(m,j) * H[j] * H[m-j] .
    The arrays are filled on demand and cached so that repeated
    calls with arbitrary orders of n are fast.  The implementation
    uses only integer arithmetic (no floating point) and works for
    all integer build types (8‑, 16‑, 32‑, 64‑, 128‑bit or arbitrary
    precision).  It is valid C11 and C++17 (static storage only,
    no ‘new’, templates or non‑POD types are used).
*/

#include <stddef.h>

#define MAX_CACHE 20000        /* safe upper bound for cache size */

static seq_t g[MAX_CACHE+1];   /* tangent numbers g[n] (g[0] unused)   */
static seq_t h[MAX_CACHE+1];   /* a(n)  (E.G.F. coefficients)       */
static seq_t d[MAX_CACHE+1];   /* helper array for D[n]              */
static idx_t cache_size = 0;   /* number of terms that have been computed (cache_size>0 after init) */

/* ensure base values are initialized */
static void init_cache(void)
{
    if (cache_size != 0) return;

    h[0] = (seq_t)1;      /* a(0) = 1 */
    d[0] = (seq_t)1;      /* D[0] = h[0]*h[0] = 1 */
    g[0] = (seq_t)0;      /* unused but defined */
    cache_size = 1;       /* we have computed up to index 0 */
}

/* extend the cached arrays up to index n (inclusive) */
static void extend_cache(idx_t n)
{
    if (n < cache_size) return;

    init_cache();

    for (idx_t k = cache_size; k <= n; ++k) {
        /* compute tangent numbers g[k]  */
        if (k == 1) {
            g[1] = (seq_t)1;          /* g[1] = 1 */
        } else {
            seq_t sum = (seq_t)0;
            seq_t bin = (seq_t)1;      /* binom(k-1,0) == 1 */
            for (idx_t a = 0; a < k; ++a) {
                seq_t term = bin * g[a] * g[k-1-a];
                sum += term;
                if (a+1 < k) {
                    /* update binom(k-1,a+1) from binom(k-1,a) */
                    bin = bin * (idx_t)(k-1-a) / (idx_t)(a+1);
                }
            }
            g[k] = sum;
        }

        /* compute h[k]  (a(k)) */
        if (k == 0) continue;        /* already set */
        seq_t sumh = (seq_t)0;
        seq_t binh = (seq_t)1;        /* binom(k,0) */
        for (idx_t i = 1; i <= k; ++i) {
            binh = binh * (idx_t)(k - i + 1) / (idx_t)i;  /* binom(k,i) */
            if (g[i] != (seq_t)0) {
                seq_t contrib = binh * g[i] * d[k - i];
                sumh += contrib;
            }
        }
        h[k] = sumh;

        /* compute d[k]  (helper) */
        seq_t sumd = (seq_t)0;
        seq_t bind = (seq_t)1;        /* binom(k,0) */
        for (idx_t j = 0; j <= k; ++j) {
            sumd += bind * h[j] * h[k - j];
            if (j < k) {
                bind = bind * (idx_t)(k - j) / (idx_t)(j + 1);
            }
        }
        d[k] = sumd;
    }
    cache_size = n + 1;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    extend_cache(n);
    return h[n];
}
