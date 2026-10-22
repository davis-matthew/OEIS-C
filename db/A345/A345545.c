/*
 * A345545  Numbers that are the sum of nine cubes in six or more ways.
 * https://oeis.org/A345545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers that are the sum of nine cubes in six or more distinct
 *            unordered ways * (found by exhaustive DP over unordered
 *            multisets of 9 positive cubes)
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes all such numbers up to a fixed bound
 * (currently 210000) and stores the first 20000 results.  For any call
 * a(idx_t n) with 1 ≤ n ≤ cand_len the n‑th value is returned.
 * If n exceeds the table size the function returns 0. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_SUM 210000     /* upper bound for the sums that are examined   */
#define K 9                /* number of cubes in each representation      */
#define CAP 6              /* saturation limit for counting ways          */
#define TABLE_SIZE 20000   /* guaranteed number of terms we store         */

static seq_t cand[TABLE_SIZE];   /* table of qualifying values                */
static size_t cand_len = 0;      /* number of values stored in cand           */

/* Build the table once.  The DP dp[k][s] counts unordered ways to form
 * the sum s using exactly k cubes (saturated at CAP).  We use only
 * integer types that are safe for all target builds.         */
static void build_candidates(void)
{
    if (cand_len != 0) return;                  /* already done */

    static uint8_t dp[K + 1][MAX_SUM + 1] = {0};
    dp[0][0] = 1;

    for (size_t m = 1;; ++m) {
        unsigned long long val_ll = (unsigned long long)m * m * m;
        if (val_ll > (unsigned long long)MAX_SUM) break;
        size_t val = (size_t)val_ll;

        for (size_t s = val; s <= (size_t)MAX_SUM; ++s) {
            size_t prev_index = s - val;
            for (size_t k = 1; k <= (size_t)K; ++k) {
                uint8_t prev = dp[k - 1][prev_index];
                if (!prev) continue;
                uint8_t cur = dp[k][s];
                uint8_t sum = cur + prev;
                if (sum > CAP) sum = CAP;
                dp[k][s] = sum;
            }
        }
    }

    /* Extract numbers with at least 6 representations.            */
    for (size_t s = 1; s <= (size_t)MAX_SUM && cand_len < TABLE_SIZE; ++s) {
        if (dp[K][s] >= CAP) {
            cand[cand_len++] = (seq_t)s;
        }
    }
}

seq_t a(idx_t n)
{
    /* a(1) is the first term listed; for n==0 we return 0 for safety. */
    if (n == 0) return 0;

    static bool built = false;
    if (!built) {
        build_candidates();
        built = true;
    }

    if ((idx_t)cand_len < n) return 0;   /* out of range */
    return cand[n - 1];
}
