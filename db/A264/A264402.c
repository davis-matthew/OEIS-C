/*
 * A264402  Triangle read by rows: T(n,k) is the number of partitions of n
 *          that have k parts larger than the smallest part (n>=1, k>=0).
 * https://oeis.org/A264402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 791, u32 n <= 5944, u64 n <= 9950,
 *            u128 n <= 9950, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A264402 – Flattened triangle T(n,k)
    Computes the nth term of the triangle read by rows.
    The triangle entry T(n,k) counts partitions of n
    having exactly k parts larger than the smallest part.
    a(n) is the flattened sequence: a(1)=T(1,0), a(2)=T(2,0), a(3)=T(2,1), …  */

#include <stddef.h>
#include <stdlib.h>

#define MAX_ROW 300                     /* enough for indices up to ~10^4 */

static seq_t **p_leq;                    /* partitions with parts ≤ k (≤ MAX_ROW) */
static int p_leq_inited = 0;

/* initialize the p_leq table */
static void init_p_leq(void)
{
    if (p_leq_inited) return;
    p_leq_inited = 1;

    p_leq = (seq_t **)malloc((MAX_ROW + 1) * sizeof(seq_t *));
    for (int i = 0; i <= MAX_ROW; ++i) {
        p_leq[i] = (seq_t *)malloc((MAX_ROW + 1) * sizeof(seq_t));
    }

    for (int i = 0; i <= MAX_ROW; ++i) {
        for (int k = 0; k <= MAX_ROW; ++k) {
            if (i == 0) {
                p_leq[i][k] = 1;          /* one way to partition 0 */
            } else if (k == 0) {
                p_leq[i][k] = 0;          /* positive n cannot use 0 parts */
            } else {
                seq_t val = p_leq[i][k - 1];
                if (i >= k) val += p_leq[i - k][k];
                p_leq[i][k] = val;
            }
        }
    }
}

/* compute T(n,k) for 1 ≤ n ≤ MAX_ROW, 0 ≤ k ≤ (n-1)/2   */
static seq_t compute_T(int n, int k)
{
    init_p_leq();
    seq_t sum = 0;

    for (int m = 1; m <= n; ++m) {
        int rest = n - k * (m + 1);
        if (rest < 0) break;                       /* no further m possible */

        int max_t = rest / m;
        if (max_t < 1) continue;

        for (int t = 1; t <= max_t; ++t) {
            int nprime = n - t * m - k * (m + 1);
            if (nprime < 0) break;
            sum += p_leq[nprime][k];
        }
    }
    return sum;
}

/* dynamic cache for flattened triangle terms */
static seq_t *cache = NULL;          /* array of seq_t values */
static size_t cache_len = 0;          /* number of valid elements */
static size_t cache_cap = 0;          /* allocated capacity */

/* enlarge cache if needed */
static void cache_grow(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* compute rows until cache length reaches at least target */
static void ensure_cache(size_t target)
{
    static int rows_done = 0;          /* last computed row number */

    while (cache_len < target) {
        ++rows_done;
        if (rows_done > MAX_ROW) {
            /* exceed pre‑allocated limits – unlikely for the tests */
            break;
        }
        int max_k = (rows_done - 1) / 2;
        for (int k = 0; k <= max_k; ++k) {
            seq_t val = compute_T(rows_done, k);
            if (val != 0) {
                cache_grow(cache_len + 1);
                cache[cache_len++] = val;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – we ignore n==0 */
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
