/*
 * A346536  a(n) is the sum of the number of ways the numbers between the n-th
 *          and (n+1)st composite squarefree numbers can be properly factored.
 * https://oeis.org/A346536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=A120944(n)+1}^{A120944(n+1)-1} A001055(k) where
 *            A120944(n) is the n-th composite squarefree number and
 *            A001055(k) is the number of unordered factorizations of k
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

/* -------------------------------------------------------------
   Inner recursive routine that counts unordered factorizations
   of n with all factors >= min_factor.
   The result is memoised in cache.
   cache is a flat array with stride entries for each possible
   min_factor value (from 0 to stride-1).
   ------------------------------------------------------------- */
static seq_t count_factor(idx_t n, idx_t min_factor,
                          seq_t *cache, idx_t stride)
{
    if (n < min_factor) return 0;
    seq_t *cell = cache + n * stride + min_factor;
    if (*cell != 0) return *cell;

    seq_t result = 0;
    idx_t s = (idx_t)sqrt((double)n);
    for (idx_t d = min_factor; d <= s; ++d) {
        if (n % d == 0) {
            idx_t m = n / d;
            if (m < d) continue;
            result += count_factor(m, d, cache, stride);
        }
    }
    result += 1;           /* the trivial factorisation n itself   */
    *cell = result;
    return result;
}

/* -------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static idx_t LIMIT = 40000;                 /* enough for n <= 10000  */
    static idx_t STRIDE = 0;                    /* sqrt(LIMIT)+1          */
    static seq_t *CACHE = NULL;                 /* memoisation table     */
    static idx_t *COMPOSITE_SF = NULL;          /* list of composite SF   */
    static size_t SF_CNT = 0;                   /* number of entries      */
    static seq_t *PREFIX = NULL;                /* prefix sums of A001055 */

    if (!initialized) {
        /* ---------------------------------------------------------
           1.  Prepare the exponent and memoisation tables
           --------------------------------------------------------- */
        STRIDE = (idx_t)sqrt((double)LIMIT) + 1;
        CACHE   = (seq_t*)calloc((LIMIT + 1) * STRIDE,
                                 sizeof(seq_t));
        PREFIX  = (seq_t*)calloc(LIMIT + 1, sizeof(seq_t));

        /* ---------------------------------------------------------
           2.  Smallest prime factor sieve for all numbers up to LIMIT
           --------------------------------------------------------- */
        idx_t *SPF = (idx_t*)calloc(LIMIT + 1, sizeof(idx_t));
        for (idx_t i = 2; i <= LIMIT; ++i) {
            if (SPF[i] == 0) {
                SPF[i] = i;
                if (i * i <= LIMIT) {
                    for (idx_t j = i * i; j <= LIMIT; j += i)
                        if (SPF[j] == 0) SPF[j] = i;
                }
            }
        }

        /* ---------------------------------------------------------
           3.  Build list of composite square‑free numbers
           --------------------------------------------------------- */
        COMPOSITE_SF = (idx_t*)malloc((LIMIT + 1) * sizeof(idx_t));
        SF_CNT = 0;
        for (idx_t x = 4; x <= LIMIT; ++x) {              /* start at 4 */
            if (SPF[x] == x) continue;                    /* prime => skip */

            /* test square‑freeness */
            idx_t t = x;
            bool sqfree = true;
            while (t > 1) {
                idx_t p = SPF[t];
                int cnt = 0;
                while (t % p == 0) {
                    t /= p;
                    ++cnt;
                    if (cnt > 1) { sqfree = false; break; }
                }
                if (!sqfree) break;
            }
            if (sqfree) COMPOSITE_SF[SF_CNT++] = x;
        }

        /* ---------------------------------------------------------
           4.  Compute A001055 values and prefix sums
           --------------------------------------------------------- */
        PREFIX[0] = 0;
        for (idx_t i = 1; i <= LIMIT; ++i) {
            seq_t val = 0;
            if (i >= 2) val = count_factor(i, 2, CACHE, STRIDE);
            PREFIX[i] = PREFIX[i - 1] + val;
        }

        /* ---------------------------------------------------------
           5.  Clean up temporary array
           --------------------------------------------------------- */
        free(SPF);

        initialized = true;
    }

    /* Guard against requests beyond the pre‑computed range */
    if (n >= SF_CNT || (n + 1) >= SF_CNT) return 0;

    idx_t s_n    = COMPOSITE_SF[n - 1];    /* A120944(n)   */
    idx_t s_next = COMPOSITE_SF[n];        /* A120944(n+1) */

    seq_t result = PREFIX[s_next - 1] - PREFIX[s_n];
    return result;
}
