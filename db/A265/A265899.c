/*
 * A265899  After a(1) = 1, positions of descents in A265894.
 * https://oeis.org/A265899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 1503, u32 n <= 1503, u64 n <= 1503,
 *            u128 n <= 1503, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A265899: positions of descents in A265894.
 *
 * The msd(n) is the most significant digit in factorial‑base of
 * (2n)! / n!.  We store all indices i such that
 *   msd(i) <= msd(i-1)   (interpreting msd(0) = 0).
 * The i‑th term of the sequence a(i) is the i‑th such index.
 *
 * The implementation uses logarithms to avoid huge integers.
 * For each new n it updates the logarithm of m_n = (2n)!/n!,
 * finds k = max{k : k! <= m_n} via a simple linear increment,
 * then finds msd(n) by binary search on the inequality
 *   m_n >= d * k!
 * using log values.  This keeps all arithmetic within double
 * precision and fits msd(n) in a small integer type.
 *
 * The sequence is built lazily: repeated calls with arbitrary
 * index are handled by extending a small cache containing the
 * recorded indices.  The algorithm is O(n log n) per new term.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Forward declaration for build systems that define these types. */
extern seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* Precomputation of logarithm of factorials.                            */
/* --------------------------------------------------------------------- */
#define MAX_LOG 200000          /* precompute log(n!) up to 200000! */

static double *log_fact = NULL; /* log_fact[i] = log(i!)  */
static bool init_done = false;

static void init_once(void)
{
    if (init_done) return;
    init_done = true;

    log_fact = (double*)malloc((MAX_LOG + 1) * sizeof(double));
    log_fact[0] = 0.0;
    for (size_t i = 1; i <= MAX_LOG; ++i)
        log_fact[i] = log_fact[i - 1] + log((double)i);
}

/* --------------------------------------------------------------------- */
/* Cache for the a(n) values.                                            */
/* --------------------------------------------------------------------- */
static size_t cache_len  = 0;    /* number of values stored so far     */
static size_t cache_cap  = 0;    /* allocated capacity                 */
static seq_t *cache_ptr  = NULL;

static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache_ptr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);          /* out of memory */
    cache_ptr = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Main function: returns a(n).                                           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t N)
{
    if (N == 0) return 0;          /* sequence starts at index 1 */

    init_once();

    /* State retained across calls. */
    static idx_t last_processed = 0;   /* highest n already processed   */
    static double log_m = 0.0;         /* log(m_n) for current n       */
    static idx_t k = 0;                /* largest k with k! <= m_n     */
    static seq_t prev_ms = 0;           /* msd of previous n            */

    /* Extend the cache until we have at least N terms. */
    while (cache_len < (size_t)N) {
        /* Next n to process */
        idx_t n = ++last_processed;

        /* Update log(m_n) for this n */
        if (n == 1) {
            log_m = log(2.0);                    /* m_1 = 2! / 1! = 2 */
        } else {
            log_m += log(2.0 * n) + log(2.0 * n - 1.0) - log((double)n);
        }

        /* Find k := max{k : k! <= m_n} */
        while (k + 1 <= 2 * n && log_fact[k + 1] <= log_m + 1e-12)
            ++k;

        /* Binary search for msd: largest d with d*k! <= m_n */
        idx_t lo = 1, hi = k, msd = 1;
        while (lo <= hi) {
            idx_t mid = lo + (hi - lo) / 2;
            double test = log_fact[k] + log((double)mid);
            if (test <= log_m + 1e-12) {
                msd = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        /* Record descent if appropriate */
        if (n == 1 || (seq_t)msd <= prev_ms) {
            ensure_cache_cap(cache_len + 1);
            cache_ptr[cache_len++] = n;
        }

        /* Update prev_ms for next iteration */
        prev_ms = (seq_t)msd;
    }

    return cache_ptr[N - 1];
}
