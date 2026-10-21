/*
 * A254950  Number of length 3 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5, 7 or 11.
 * https://oeis.org/A254950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of length‑3 arrays from 1..n+1 with all leading partial sums
   divisible by 2, 3, 5, 7 or 11.  The algorithm precomputes which
   numbers are “good” (divisible by any of those primes) and then, for
   each possible sum s = a + b, counts suitable a’s and c’s in O(n). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n) {
    if (n < 1) return 0;                      /* undefined below offset */

    /* Upper bound for single elements */
    idx_t M = n + 1;                          /* elements are 1..M */

    /* We will need good numbers up to 3*M */
    size_t maxSum = 3 * (size_t)M;

#ifdef __cplusplus
    /* C++ implementation using std::vector */
    std::vector<bool> good(maxSum + 1, false);
    for (size_t x = 1; x <= maxSum; ++x) {
        if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 ||
            x % 7 == 0 || x % 11 == 0) {
            good[x] = true;
        }
    }

    std::vector<seq_t> pref(M + 1, 0);          /* prefix of good among 1..M   */
    for (size_t i = 1; i <= M; ++i) {
        pref[i] = pref[i - 1] + (good[i] ? 1 : 0);
    }

    std::vector<seq_t> cum(maxSum + 1, 0);      /* cumulative good up to maxSum */
    for (size_t i = 1; i <= maxSum; ++i) {
        cum[i] = cum[i - 1] + (good[i] ? 1 : 0);
    }

    seq_t result = 0;

    for (idx_t s = 1; s <= 2 * M; ++s) {        /* s = a + b               */
        if (!good[(size_t)s]) continue;

        idx_t L = (s > M) ? (s - M) : 1;        /* possible a values       */
        idx_t R = s - 1;
        if (R > M) R = M;
        if (L > R) continue;

        seq_t countA = pref[(size_t)R] - pref[(size_t)(L - 1)];
        seq_t countC = cum[(size_t)(s + M)] - cum[(size_t)s];
        result += countA * countC;
    }

    return result;
#else
    /* C implementation using malloc / free */
    bool *good = calloc(maxSum + 1, sizeof(bool));
    if (!good) return 0;
    for (size_t x = 1; x <= maxSum; ++x) {
        good[x] = (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 ||
                   x % 7 == 0 || x % 11 == 0);
    }

    seq_t *pref = calloc(M + 1, sizeof(seq_t));
    if (!pref) { free(good); return 0; }
    for (size_t i = 1; i <= M; ++i) {
        pref[i] = pref[i - 1] + (good[i] ? 1 : 0);
    }

    seq_t *cum = calloc(maxSum + 1, sizeof(seq_t));
    if (!cum) { free(good); free(pref); return 0; }
    for (size_t i = 1; i <= maxSum; ++i) {
        cum[i] = cum[i - 1] + (good[i] ? 1 : 0);
    }

    seq_t result = 0;
    for (idx_t s = 1; s <= 2 * M; ++s) {
        if (!good[(size_t)s]) continue;

        idx_t L = (s > M) ? (s - M) : 1;
        idx_t R = s - 1;
        if (R > M) R = M;
        if (L > R) continue;

        seq_t countA = pref[(size_t)R] - pref[(size_t)(L - 1)];
        seq_t countC = cum[(size_t)(s + M)] - cum[(size_t)s];
        result += countA * countC;
    }

    free(good);
    free(pref);
    free(cum);
    return result;
#endif
}
