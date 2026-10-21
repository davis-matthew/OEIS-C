/*
 * A244243  Number of partitions of n into 7 parts such that every i-th
 *          smallest part (counted with multiplicity) is different from i.
 * https://oeis.org/A244243
 *
 * Offset:    35    (a(35) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 82, u32 n <= 500, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A244243 – Number of partitions of n into 7 parts such that every
    i‑th smallest part (counted with multiplicity) is different from i.
    Formula (conjectured recurrence, valid for n > 77):

       a(n) = a(n-1)+a(n-2)-a(n-5)-a(n-7)-a(n-8)+a(n-10)+a(n-11)
              +2*a(n-12)-2*a(n-16)-a(n-17)-a(n-18)+a(n-20)+a(n-21)
              +a(n-23)-a(n-26)-a(n-27)+a(n-28)

    Initial values for n = 35 … 77 are computed by brute force. */

/* ------------------------------------- */
#include <stddef.h>   /* for size_t, idx_t, seq_t are defined by the build */

#define MAX_N 20000   /* sufficient for indexes up to ~10000 */

/* -------------------------------------------------- */
/*  Brute‑force DFS that counts the admissible partitions. */
static seq_t dfs(idx_t idx, idx_t prev, idx_t rem_sum, idx_t rem_parts)
{
    if (rem_parts == 0)
        return (rem_sum == 0) ? (seq_t)1 : (seq_t)0;

    /* minimal possible sum if all remaining parts equal 'prev' */
    if (rem_sum < prev * rem_parts)
        return (seq_t)0;

    seq_t total = (seq_t)0;
    idx_t max_val = rem_sum / rem_parts;   /* largest allowed value */

    for (idx_t v = prev; v <= max_val; ++v) {
        if (v == idx)          /* violation: i‑th part must not equal i */
            continue;
        total += dfs(idx + 1, v, rem_sum - v, rem_parts - 1);
    }
    return total;
}

static seq_t compute_small(idx_t n)
{
    return dfs((idx_t)1, (idx_t)1, n, (idx_t)7);
}

/* -------------------------------------------------- */
/*  Cache for computed values.                           */
static seq_t cache[MAX_N + 1];
static unsigned char init = 0;   /* 0 = not yet pre‑computed    */
static idx_t computed_to = 0;   /* largest n that has been filled */

/* -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < (idx_t)35)                /* behaviour below offset is irrelevant */
        return (seq_t)0;

    if (!init) {
        /* Pre‑compute the first 43 values (35 … 77) by brute force. */
        for (idx_t i = (idx_t)35; i <= (idx_t)77; ++i)
            cache[i] = compute_small(i);
        init = 1;
        computed_to = (idx_t)77;
    }

    if (n <= computed_to)
        return cache[n];

    /* Compute missing values up to n using the recurrence. */
    for (idx_t i = computed_to + 1; i <= n; ++i) {
#if SIZEOF_LONG_LONG > 4
        /* For seq_t that fits in 64‑bit we use a wider intermediate
           type (__uint128_t) to avoid overflow in the summations. */
        if (sizeof(seq_t) <= 8) {
            __uint128_t P = (__uint128_t)cache[i-1] + cache[i-2]
                          + cache[i-10] + cache[i-11]
                          + ((__uint128_t)cache[i-12] << 1)
                          + cache[i-20] + cache[i-21]
                          + cache[i-23] + cache[i-28];

            __uint128_t N = (__uint128_t)cache[i-5] + cache[i-7]
                          + cache[i-8] + ((__uint128_t)cache[i-16] << 1)
                          + cache[i-17] + cache[i-18]
                          + cache[i-26] + cache[i-27];

            cache[i] = (seq_t)(P - N);
        } else
#endif
        {
            /* seq_t is arbitrary‑precision or larger than 64‑bit. */
            seq_t P = cache[i-1] + cache[i-2]
                      + cache[i-10] + cache[i-11]
                      + (cache[i-12] << 1)
                      + cache[i-20] + cache[i-21]
                      + cache[i-23] + cache[i-28];

            seq_t N = cache[i-5] + cache[i-7]
                      + cache[i-8] + (cache[i-16] << 1)
                      + cache[i-17] + cache[i-18]
                      + cache[i-26] + cache[i-27];

            cache[i] = P - N;
        }
    }
    computed_to = n;
    return cache[n];
}
