/*
 * A341244  Expansion of (-1 + Product_{k>=1} 1 / (1 + (-x)^k))^5.
 * https://oeis.org/A341244
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in (f(x)-1)^5, where f(x) =
 *            ∏_{k≥1} (1 + x^{2k-1}) (distinct odd parts)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_max = 0;

/* Compute all a(k) for 0 ≤ k ≤ N and store them in cache */
static void compute_to(idx_t N)
{
    if (N <= cache_max) return;

    /* Allocate new cache */
    seq_t *new_cache = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!new_cache) return;              /* OOM – unlikely in test harness */
    for (idx_t i = 0; i <= N; ++i) new_cache[i] = 0;

    /* f : partitions into distinct odd parts, f[0] = 1 */
    seq_t *f  = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *p2 = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *p3 = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *p4 = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *p5 = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!f || !p2 || !p3 || !p4 || !p5) {
        free(f); free(p2); free(p3); free(p4); free(p5);
        free(new_cache);
        return;
    }

    for (idx_t i = 0; i <= N; ++i) f[i] = 0;
    f[0] = 1;
    for (idx_t m = 1; m <= N; m += 2)          /* odd parts only */
        for (idx_t j = N; j >= m; --j)
            f[j] = f[j] + f[j - m];

    /* Helper to perform convolution: res = a * b (polynomials up to N) */
#define CONV(a,b,res)                          \
    do {                                       \
        for (idx_t i = 0; i <= N; ++i) res[i] = 0;  \
        for (idx_t i = 0; i <= N; ++i) {        \
            if ((a)[i] == 0) continue;         \
            for (idx_t j = 0; i + j <= N; ++j) {  \
                if ((b)[j] == 0) continue;      \
                res[i + j] = res[i + j] + (a)[i] * (b)[j]; \
            }                                   \
        }                                       \
    } while (0)

    CONV(f, f, p2);          /* p2 = f^2 */
    CONV(p2, f, p3);         /* p3 = f^3 */
    CONV(p3, f, p4);         /* p4 = f^4 */
    CONV(p4, f, p5);         /* p5 = f^5 */

#undef CONV

    typedef seq_t Small;
    const Small c5  = (Small)5;
    const Small c10 = (Small)10;

    for (idx_t i = 1; i <= N; ++i) {
        seq_t val = p5[i];                     /*  f^5  */
        val = val - (p4[i] * c5);              /* -5f^4 */
        val = val + (p3[i] * c10);             /* +10f^3*/
        val = val - (p2[i] * c10);             /* -10f^2*/
        val = val + (f[i] * c5);               /* +5 f  */
        new_cache[i] = val;
    }
    new_cache[0] = 0;     /* a(0) is defined as 0 */

    free(f); free(p2); free(p3); free(p4); free(p5);

    if (cache) free(cache);
    cache = new_cache;
    cache_max = N;
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* behaviour below offset is irrelevant */
    compute_to(n);
    return cache[n];
}
