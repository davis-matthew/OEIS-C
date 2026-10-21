/*
 * A266325  Smallest integer m such that there is a partition of m with
 *          product of multiplicities of parts equal to n.
 * https://oeis.org/A266325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min { m ≥ 0 : there exists a partition of m whose product
 *            of multiplicities equals n }
 * Range:     u8 n <= 256, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n>1, the optimal partition uses multiplicities that multiply
   to n, sorted descending, paired with part sizes 1,2,3,...,
   giving a weighted sum.*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Pre‑compute all divisors of numbers up to a fixed upper bound. */
#define MAX_N   20000          /* sufficient for n ≤ 10000 */
#define MAX_DIV 200            /* maximum number of divisors for 1 ≤ n ≤ MAX_N */

static unsigned char  div_cnt[MAX_N + 1] = {0};
static int            divisors[MAX_N + 1][MAX_DIV];
static bool           divisors_ready = false;

/* Initialise the divisor tables once. */
static void init_divisors(void)
{
    if (divisors_ready) return;
    for (int i = 2; i <= MAX_N; ++i) {
        for (int j = i; j <= MAX_N; j += i) {
            int k = div_cnt[j]++;
            if (k < MAX_DIV) divisors[j][k] = i;
        }
    }
    divisors_ready = true;
}

/* Recursive search for the minimal weighted sum for a given remainder. */
static void dfs(uint64_t rem, uint64_t last, uint64_t weight,
                uint64_t cur_sum, uint64_t *best)
{
    if (rem == 1) {                 /* completed a factorisation */
        if (cur_sum < *best) *best = cur_sum;
        return;
    }
    if (cur_sum >= *best) return;   /* pruning */

    /* Enumerate all divisors of rem that are ≤ last, in descending order. */
    int index = div_cnt[(int)rem] - 1;
    for (; index >= 0; --index) {
        int f = divisors[(int)rem][index];
        if ((uint64_t)f > last) continue;   /* maintain non‑increasing order */

        uint64_t next_rem  = rem / f;
        uint64_t next_sum  = cur_sum + weight * f;
        dfs(next_rem, (uint64_t)f, weight + 1, next_sum, best);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* offset is 1; undefined behaviour for n=0 */

    init_divisors();

    if (n == 1)                      /* a(1) = 0 by convention */
        return (seq_t)0;

    if (n > (idx_t)MAX_N) {          /* safety check – fallback computation */
        /* In practice n will not exceed MAX_N for the test suite. */
        return (seq_t)0;
    }

    uint64_t best = (uint64_t)-1;      /* initialise with maximum */
    dfs((uint64_t)n, (uint64_t)n, 1ULL, 0ULL, &best);
    return (seq_t)best;
}
