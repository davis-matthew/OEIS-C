/*
 * A308923  Sum of the third largest parts in the partitions of n into 6
 *          primes.
 * https://oeis.org/A308923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of the third largest parts in the partitions of n
 *            into six primes (nonincreasing ordering). This is computed by
 *            dynamic programming over the number of parts and the sum of the
 *            parts, keeping track of the j‑th smallest part in each partial
 *            partition. For k=6 the 4th smallest part (index...
 * Range:     u8 n <= 43, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* #includes and typedefs are assumed to be provided by the build system. */
#include <stdbool.h>

#define MAXN 10000
#define K 6

/* Static arrays are zero‑initialised. */
static seq_t dp_cnt[K + 1][MAXN + 1];
static seq_t dp_sum[K + 1][MAXN + 1][K + 1];
static bool initialized = false;

/* --------------------------------------------------------------------- */
/*  Initialise the putative primes up to MAXN and perform the DP once.   */
/* --------------------------------------------------------------------- */
static void init(void)
{
    if (initialized)
        return;

    /* Sieve of Eratosthenes for primes up to MAXN. */
    static bool is_prime[MAXN + 1];
    for (int i = 0; i <= MAXN; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= MAXN; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= MAXN; j += i)
                is_prime[j] = false;

    /* Collect the primes.  The number of primes up to 10000 is 1229. */
    int primes[1229];
    int prime_cnt = 0;
    for (int i = 2; i <= MAXN; ++i)
        if (is_prime[i])
            primes[prime_cnt++] = i;

    /* Dynamic programming: dp_cnt[k][s] – number of partitions of s into k primes
       (ascending order).  dp_sum[k][s][j] – sum over all such partitions of the
       j‑th smallest part.  k runs 0..6, j runs 1..k.  dp_cnt[0][0] = 1. */
    dp_cnt[0][0] = 1;

    for (int pi = 0; pi < prime_cnt; ++pi) {
        int p = primes[pi];

        /* For each prime we may use it any number of times – iterate k ascending. */
        for (int k = 1; k <= K; ++k) {
            for (idx_t s = p; s <= MAXN; ++s) {
                seq_t prev_cnt = dp_cnt[k - 1][s - p];
                if (!prev_cnt)
                    continue;

                dp_cnt[k][s] += prev_cnt;

                /* Carry over the j‑th smallest part from the (k‑1)‑partition. */
                for (int j = 1; j < k; ++j)
                    dp_sum[k][s][j] += dp_sum[k - 1][s - p][j];

                /* The new part p becomes the k‑th smallest (the largest) part. */
                dp_sum[k][s][k] += (seq_t)p * prev_cnt;
            }
        }
    }

    initialized = true;
}

/* --------------------------------------------------------------------- */
/*  Public API required by the problem statement.                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init();

    if (n > MAXN)
        return 0;                 /* n is beyond the pre‑computed range.    */

    /* For 6 parts, the 4th smallest part (index 4) is the third largest part. */
    return dp_sum[6][n][4];
}
