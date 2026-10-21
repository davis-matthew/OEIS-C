/*
 * A280194  Expansion of 1/(1 - Sum_{k>=1} mu(k)^2*x^k), where mu(k) is the
 *          Moebius function (A008683).
 * https://oeis.org/A280194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into squarefree parts
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 68, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute floor(sqrt(n)) for idx_t using binary search.
   Uses unsigned __int128 to avoid overflow. */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Memoised DP table */
    static seq_t *dp = NULL;        /* dp[0..max_n] */
    static size_t dp_cap = 0;
    static idx_t max_n = 0;

    /* Square‑free marker and list */
    static unsigned char *is_sf = NULL; /* is_sf[i] == 1 if i is squarefree */
    static size_t sf_cap = 0;
    static size_t *sf_list = NULL;     /* list of all squarefree numbers ≤ current max */
    static size_t sf_cnt = 0;          /* size of sf_list */
    static size_t sf_list_cap = 0;

    /* Base case: a(0) = 1 */
    if (n == 0) {
        if (dp_cap == 0) {
            dp = (seq_t *)malloc(sizeof(seq_t));
            if (!dp) return (seq_t)0; /* allocation failure */
            dp[0] = (seq_t)1;
            dp_cap = 1;
        }
        return dp[0];
    }

    /* Expand data structures if necessary */
    if (n > max_n) {
        /* 1. Expand DP buffer */
        size_t new_cap = (size_t)n + 1;
        if (new_cap > dp_cap) {
            seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* allocation failure */
            /* Initialise new slots to zero */
            for (size_t i = dp_cap; i < new_cap; ++i)
                tmp[i] = (seq_t)0;
            dp = tmp;
            dp_cap = new_cap;
        }

        /* 2. Build square‑free marker up to n */
        if (new_cap > sf_cap) {
            unsigned char *tmp_sf = (unsigned char *)malloc(new_cap);
            if (!tmp_sf) return (seq_t)0;
            for (size_t i = 0; i < new_cap; ++i)
                tmp_sf[i] = 1;
            tmp_sf[0] = 0;            /* 0 is not considered squarefree */

            /* Erase numbers divisible by a square */
            idx_t lim = isqrt_idx(n);
            for (idx_t p = 2; p <= lim; ++p) {
                if (tmp_sf[p]) {          /* p is prime */
                    size_t sq = (size_t)p * (size_t)p;
                    for (size_t j = sq; j < new_cap; j += sq)
                        tmp_sf[j] = 0;
                }
            }

            free(is_sf);
            is_sf = tmp_sf;
            sf_cap = new_cap;
        }

        /* 3. Build list of squarefree numbers up to n */
        sf_cnt = 0;
        for (size_t i = 1; i <= (size_t)n; ++i)
            if (is_sf[i]) ++sf_cnt;

        if (sf_cnt > sf_list_cap) {
            size_t *tmp_list = (size_t *)realloc(sf_list, sf_cnt * sizeof(size_t));
            if (!tmp_list) return (seq_t)0;
            sf_list = tmp_list;
            sf_list_cap = sf_cnt;
        }

        size_t idx = 0;
        for (size_t i = 1; i <= (size_t)n; ++i)
            if (is_sf[i]) sf_list[idx++] = i;

        /* 4. Compute DP values up to n */
        for (size_t i = (size_t)max_n + 1; i <= (size_t)n; ++i) {
            seq_t sum = (seq_t)0;
            for (size_t s = 0; s < sf_cnt; ++s) {
                size_t k = sf_list[s];
                if (k > i) break;
                sum += dp[i - k];
            }
            dp[i] = sum;
        }

        max_n = n;
    }

    return dp[(size_t)n];
}
