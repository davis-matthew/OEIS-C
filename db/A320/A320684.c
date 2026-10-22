/*
 * A320684  Number of 9-element subsets of [n] having a prime element sum.
 * https://oeis.org/A320684
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 9‑element subsets of [n] having a prime sum
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 63, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* static state                                                     */
/* ------------------------------------------------------------------ */
static idx_t curN = 0;          /* largest processed n                  */
static seq_t *res   = NULL;     /* memoised a(n) values                 */
static size_t res_cap = 0;      /* capacity of res                      */

/* dp[k][s] = number of k‑subsets of [1..curN] summing to s           */
/* k = 0 … 8                                                          */
static seq_t *dp[9]   = {NULL};
static size_t dp_cap[9] = {0};

/* prime sieve up to current limit (inclusive)                         */
static bool *prime_flag = NULL;
static size_t prime_cap = 0;

/* ------------------------------------------------------------------ */
/* Grow an array of seq_t                                             */
/* ------------------------------------------------------------------ */
static void grow_seq_arr(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    seq_t *tmp = (seq_t *)realloc(*arr, need * sizeof(seq_t));
    if (!tmp) abort();
    memset(tmp + *cap, 0, (need - *cap) * sizeof(seq_t));
    *arr = tmp;
    *cap = need;
}

/* ------------------------------------------------------------------ */
/* Build sieve of Eratosthenes up to 'limit' (inclusive)              */
/* ------------------------------------------------------------------ */
static void build_prime_sieve(idx_t limit)
{
    size_t new_cap = (size_t)limit + 1;
    if (new_cap <= prime_cap) return;           /* already enough */

    bool *new_flag = (bool *)malloc(new_cap * sizeof(bool));
    if (!new_flag) abort();
    memset(new_flag, true, new_cap * sizeof(bool));
    new_flag[0] = new_flag[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!new_flag[p]) continue;
        for (idx_t j = p * p; j <= limit; j += p) new_flag[j] = false;
    }

    free(prime_flag);
    prime_flag = new_flag;
    prime_cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/* Ensure res[] capacity                                            */
/* ------------------------------------------------------------------ */
static void ensure_res_cap(idx_t needed)
{
    if (res_cap >= (size_t)needed) return;
    size_t new_cap = (size_t)needed;
    seq_t *tmp = (seq_t *)realloc(res, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    memset(tmp + res_cap, 0, (new_cap - res_cap) * sizeof(seq_t));
    res = tmp;
    res_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main function                                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 9) return (seq_t)0;

    /* initialise dp[0] if this is the first call */
    if (curN == 0) {
        grow_seq_arr(&dp[0], &dp_cap[0], 1);
        dp[0][0] = (seq_t)1;   /* empty subset */
    }

    /* process new values up to n */
    for (idx_t i = curN + 1; i <= n; ++i)
    {
        /* enlarge dp capacities for the new element i */
        for (int k = 1; k <= 8; ++k) {
            size_t needed = (size_t)k * (size_t)i + 1;
            grow_seq_arr(&dp[k], &dp_cap[k], needed);
        }
        ensure_res_cap(i + 1);
        if (i < 9) {
            res[i] = 0;
            /* still need to update dp for completeness */
        } else {
            /* ------------------------------------------------------------------ */
            /* count 9‑subsets that contain i and have prime sum                    */
            /* ------------------------------------------------------------------ */
            seq_t add = 0;
            /* ensure prime sieve covers possible sums up to 9*i */
            if (prime_cap < (size_t)(9 * i + 1))
                build_prime_sieve(9 * i);

            idx_t maxPrev8 = (idx_t)8 * (i - 1);
            for (idx_t s = 0; s <= maxPrev8; ++s) {
                seq_t cnt = dp[8][s];
                if (cnt && prime_flag[s + i]) add += cnt;
            }
            res[i] = res[i-1] + add;
        }

        /* ------------------------------------------------------------------ */
        /* update dp arrays to include the element i                           */
        /* ------------------------------------------------------------------ */
        for (int k = 8; k >= 1; --k) {
            idx_t maxPrev = (idx_t)(k - 1) * (i - 1);   /* previous maximum sum */
            for (idx_t s = 0; s <= maxPrev; ++s) {
                seq_t cnt = dp[k-1][s];
                if (cnt) dp[k][s + i] += cnt;          /* add element i */
            }
        }
    }

    curN = n;
    return res[n];
}
