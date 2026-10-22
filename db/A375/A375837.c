/*
 * A375837  Triangle read by rows: T(n,k) is the number of rooted chains
 *          starting with the cycle (1)(2)(3)...(n) of length k of permutation
 *          poset of n letters.
 * https://oeis.org/A375837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(0) = 1. For n > 0 let m be the greatest integer with m(m+1)/2
 *            ≤ n. Let k = n – m(m+1)/2. If k == 0 return 0, otherwise a(n) =
 *            T(m, k), where T(n, k) is defined by T(n, 1) = 1 (n ≥ 1) T(n, k)
 *            = Σ_{i = k-1}^{n-1} Stirling1(n, i) * T(i, k-1) (k ≥ 2)
 *            Stirling1(n, k) denotes the unsigned Stirling nu...
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build environment.  */

static bool   init_done   = false;
static seq_t *stirl       = NULL;   /* Stirling1 table */
static seq_t *Tmat        = NULL;   /* T(n,k) table          */
static size_t stride      = 0;      /* stride = maxM + 1     */

/* Compute Stirling numbers of the first kind up to maxM */
static void compute_tables(void)
{
    const size_t maxM = 200;                    /* enough for n ≤ 10 000 */
    stride = maxM + 1;
    const size_t dim = (maxM + 1) * stride;

    stirl = (seq_t *)malloc(dim * sizeof(seq_t));
    Tmat  = (seq_t *)malloc(dim * sizeof(seq_t));
    if (!stirl || !Tmat) {                          /* simple error handling */
        exit(1);
    }

    /* Stirling numbers: Stirling1(0,0)=1, others 0. */
    for (size_t i = 0; i < dim; ++i) stirl[i] = 0;
    stirl[0] = 1;                                 /* stirl[0][0] */

    for (size_t n = 1; n <= maxM; ++n) {
        for (size_t k = 1; k <= n; ++k) {
            seq_t a = stirl[(n - 1) * stride + (k - 1)];
            seq_t b = stirl[(n - 1) * stride + k];
            stirl[n * stride + k] = a + ((seq_t)(n - 1)) * b;
        }
        for (size_t k = n + 1; k <= maxM; ++k) {
            stirl[n * stride + k] = 0;
        }
    }

    /* T(n,k) table */
    for (size_t n = 0; n <= maxM; ++n) {
        for (size_t k = 0; k <= maxM; ++k) {
            Tmat[n * stride + k] = 0;
        }
    }

    for (size_t n = 1; n <= maxM; ++n) {
        Tmat[n * stride + 1] = 1;                    /* T(n,1)=1 */
        for (size_t k = 2; k <= n; ++k) {
            seq_t sum = 0;
            for (size_t i = k - 1; i <= n - 1; ++i) {
                seq_t prod = stirl[n * stride + i] * Tmat[i * stride + (k - 1)];
                sum += prod;
            }
            Tmat[n * stride + k] = sum;
        }
    }

    init_done = true;
}

/* Find the largest m with m*(m+1)/2 ≤ n */
static idx_t find_m(idx_t n)
{
    idx_t m = 0;
    while (((unsigned __int128)(m + 1) * (m + 2)) / 2 <= n) {
        ++m;
    }
    return m;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    if (!init_done) {
        compute_tables();
    }

    idx_t m = find_m(n);
    idx_t tri = m * (m + 1) / 2;
    idx_t k   = n - tri;

    if (k == 0) {            /* n is a triangular number (except 0) */
        return (seq_t)0;
    }
    /* p = T(m, k) */
    return Tmat[m * stride + k];
}
