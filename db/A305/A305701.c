/*
 * A305701  Nonnegative integers whose decimal digits span an initial interval
 *          of {0,...,9}.
 * https://oeis.org/A305701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth non‑negative integer whose decimal digits use a
 *            contiguous initial set {0,1,…,k} of digits, for some k
 * Range:     u8 n <= 9, u16 n <= 449, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_contiguous
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_contiguous.h"

#define MASK_SIZE 1024
#define MAXLEN 120
#define LIMIT ((unsigned long long)1ULL << 60)

static unsigned long long f[MAXLEN + 1][MASK_SIZE];
static bool f_initialized = false;

/* Initialize DP array f[rem][mask] where
   f[rem][mask] = number of ways to fill rem positions (using digits 0‑9)
   starting with currently used digits 'mask', such that the final set of
   used digits is contiguous from 0. */
static void init_dp(void) {
    if (f_initialized) return;
    for (unsigned int mask = 0; mask < MASK_SIZE; ++mask) {
        f[0][mask] = is_contiguous(mask) ? 1ULL : 0ULL;
    }
    for (int rem = 1; rem <= MAXLEN; ++rem) {
        for (unsigned int mask = 0; mask < MASK_SIZE; ++mask) {
            unsigned long long sum = 0ULL;
            for (int d = 0; d <= 9; ++d) {
                unsigned int nmask = mask | (1U << d);
                sum += f[rem - 1][nmask];
                if (sum > LIMIT) { sum = LIMIT; break; }
            }
            f[rem][mask] = sum;
        }
    }
    f_initialized = true;
}

/* Main function required by the test harness */
seq_t a(idx_t n) {
    init_dp();
    if (n == 1) return (seq_t)0;          /* a(1) = 0 */

    /* exclude the zero term from rank calculation */
    idx_t rem_n = n - 1;                   /* remaining rank among positive terms */

    /* Find length L such that cumulative count up to L-1 < rem_n <= cumulative up to L */
    unsigned long long cumulative = 0ULL;
    int L = 0;
    for (int len = 1; len <= MAXLEN; ++len) {
        unsigned long long total_len = 0ULL;
        /* first digit cannot be 0: choose d=1..9 */
        for (int d = 1; d <= 9; ++d) {
            unsigned int mask = 1U << d;
            total_len += f[len - 1][mask];
            if (total_len > LIMIT) { total_len = LIMIT; break; }
        }
        if (cumulative + total_len >= rem_n) {
            L = len;
            break;
        }
        cumulative += total_len;
    }

    /* rank within numbers of length L (1‑based) */
    idx_t rank = rem_n - cumulative;
    seq_t value = 0ULL;
    unsigned int used_mask = 0U;

    for (int pos = 0; pos < L; ++pos) {
        int start_d = (pos == 0) ? 1 : 0;
        for (int d = start_d; d <= 9; ++d) {
            unsigned int new_mask = used_mask | (1U << d);
            int rem = L - pos - 1;
            unsigned long long cnt = f[rem][new_mask];
            if (cnt < rank) {
                rank -= cnt;
                continue;
            }
            value = value * 10 + (seq_t)d;
            used_mask = new_mask;
            break;
        }
    }
    return value;
}
