/*
 * A370479  G.f. satisfies A(x) = ( 1 + x * (A(x)^(1/2) / (1-x))^2 )^2.
 * https://oeis.org/A370479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A370479
 * a(0) = 1
 * a(n) = 2 * Σ_{k=0..n} binomial(2*k+2,k) * binomial(n+k-1,n-k)/(2*k+2)
 *       = Σ_{k=1..n} Catalan(k+1) * binomial(n+k-1,n-k)
 * (computed by a single loop in O(n); all arithmetic is exact)

 * The code memoises all values up to the largest index requested.
 */

#include <stdlib.h>

/* ----------- local helpers -------------------------------------------- */

/* multiplicative binomial: binom(N,K)  (conservative – exact division) */
static seq_t binom_val(idx_t N, idx_t K)
{
    if (K > N) return (seq_t)0;
    if (K == 0) return (seq_t)1;
    if (K > N - K) K = N - K;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= K; ++i) {
        res = (res * (seq_t)(N - K + i)) / (seq_t)i;
    }
    return res;
}

/* ----------- memoisation for a(n) ------------------------------------ */

static seq_t *a_cache = NULL;   /* a_cache[n] = a(n) */
static idx_t cache_len = 0;     /* number of cached entries */

/* grow the cache array to include indices up to n (inclusive) */
static void ensure_a(idx_t n)
{
    if (cache_len > n) return;
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;

    /* initialise first term if the cache was empty */
    if (cache_len == 0) {
        a_cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute all missing entries up to n */
    for (idx_t m = cache_len; m <= n; ++m) {
        if (m == 0) { /* already set */
            a_cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;

        /* Catalan(k+1) – handle first value directly */
        seq_t C_cur = (seq_t)2;   /* C_2 = 2 */
        seq_t B_k   = (seq_t)m;   /* binom(n+1-1, n-1) = binom(m, m-1) = m */

        for (idx_t k = 1; k <= m; ++k) {
            /* add the current term */
            sum += C_cur * B_k;

            if (k < m) {
                /* prepare next Catalan value: C_{k+2} from C_{k+1} */
                seq_t numC = C_cur * (seq_t)(2 * (2 * (k + 1) + 1));
                C_cur = numC / (seq_t)(k + 3);

                /* prepare next binomial B_{k+1} from B_k */
                seq_t numB = B_k * (seq_t)(m + k) * (seq_t)(m - k);
                seq_t denB = (seq_t)((2 * k + 1) * (2 * k));
                B_k = numB / denB;
            }
        }

        a_cache[m] = sum;
    }
    cache_len = new_len;
}

/* ----------- public interface ----------------------------------------- */

seq_t a(idx_t n)
{
    ensure_a(n);
    return a_cache[n];
}
