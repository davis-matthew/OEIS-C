/*
 * A317836  Number of partitions of n with carry-free sum in primorial base.
 * https://oeis.org/A317836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n with carry-free sum * in
 *            primorial base. Computed by an unbounded‑coin DP over the *
 *            digit‑sum vectors
 * Range:     u8 n <= 58, u16 n <= 388, u32 n <= 2310, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* primes used as bases for the primorial representation.
 * Only the first several are needed for n <= 10000. */
static const uint64_t _primes[20] = {
    2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
    31ULL,37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,71ULL
};

seq_t a(idx_t n)
{
    /* Special case n = 0: the empty partition */
    if (n == 0) {
        seq_t one = 1;
        return one;
    }

    /* 1. Compute the primorial (factorial‑like) digits of n
     *    (least significant digit first).  D[k] <= prime_{k+1}-1. */
    uint8_t D[20];
    int K = 0;                      /* number of digits */
    uint64_t temp = (uint64_t)n;
    do {
        D[K] = (uint8_t)(temp % _primes[K]);
        temp /= _primes[K];
        K++;
    } while (temp > 0);

    /* 2. Mixed radix multipliers for encoding a state vector into an index.
     *    mult[i] = ∏_{j=0}^{i-1} (D[j] + 1).  */
    uint64_t mult[20];
    mult[0] = 1;
    for (int i = 1; i < K; ++i) {
        mult[i] = mult[i-1] * ((uint64_t)D[i-1] + 1ULL);
    }
    uint64_t size = mult[K-1] * ((uint64_t)D[K-1] + 1ULL);

    /* 3. Allocate DP array and pre‑compute digit vectors of every state. */
    seq_t *dp = (seq_t *)calloc((size_t)size, sizeof(seq_t));
    if (!dp) return 0;          /* memory failure: return zero */

    uint8_t *state_digits = (uint8_t *)malloc((size_t)(size * K) * sizeof(uint8_t));
    if (!state_digits) { free(dp); return 0; }

    for (uint64_t idx = 0; idx < size; ++idx) {
        uint64_t tmp = idx;
        uint8_t *ptr = &state_digits[(size_t)idx * (size_t)K];
        for (int i = 0; i < K; ++i) {
            ptr[i] = (uint8_t)(tmp % ((uint64_t)D[i] + 1ULL));
            tmp /= ((uint64_t)D[i] + 1ULL);
        }
    }

    /* 4. DP initialization: empty sum. */
    dp[0] = 1;

    /* 5. Unbounded coin DP: each integer s = 1..n is a possible summand.
     *    For each s, determine its digit vector v[ ], its index shift inc,
     *    and update all states that can accept this coin. */
    for (uint64_t s = 1; s <= n; ++s) {
        uint8_t v[20];
        uint64_t tmp = s;
        for (int i = 0; i < K; ++i) {
            v[i] = (uint8_t)(tmp % _primes[i]);
            tmp /= _primes[i];
        }

        /* The coin is useless if any digit exceeds the target digit D. */
        int skip = 0;
        for (int i = 0; i < K; ++i) {
            if (v[i] > D[i]) { skip = 1; break; }
        }
        if (skip) continue;

        /* Incremental index shift for this coin. */
        uint64_t inc = 0;
        for (int i = 0; i < K; ++i) {
            inc += (uint64_t)v[i] * mult[i];
        }

        /* Iterate over all states in ascending order to allow unlimited copies
         * of the current coin. */
        for (uint64_t idx = 0; idx < size; ++idx) {
            seq_t cur = dp[idx];
            if (cur == 0) continue;

            uint8_t *stat = &state_digits[(size_t)idx * (size_t)K];
            int ok = 1;
            for (int i = 0; i < K; ++i) {
                if (stat[i] + v[i] > D[i]) { ok = 0; break; }
            }
            if (!ok) continue;

            uint64_t new_idx = idx + inc;
            dp[new_idx] += cur;
        }
    }

    /* 6. The target state is the one where all digits reach their maximum. */
    seq_t result = dp[size - 1];

    /* 7. Free allocated memory and return. */
    free(state_digits);
    free(dp);
    return result;
}
