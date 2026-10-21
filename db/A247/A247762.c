/*
 * A247762  Numbers in decimal representation, such that in Slovak their
 *          digits are in alphabetic order.
 * https://oeis.org/A247762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 1787, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th non‑negative integer whose decimal digits, when
 * interpreted with the Slovak digit names, are in non‑decreasing alphabetic
 * order.  The sequence is indexed from 1, so a(1)=0, a(2)=1, …
 *
 * Implementation details:
 *   - Count numbers ≤ X that satisfy the digit order constraint by a digit DP.
 *   - Find the n‑th term by binary search, expanding the upper bound until
 *     the count is at least n.
 *   - Works for seq_t up to 128 bits and for arbitrary‑precision builds.
 */

#include <stdint.h>

static const unsigned char rank_of[10] = {
    /* 0 */ 3,
    /* 1 */ 2,
    /* 2 */ 1,
    /* 3 */ 9,
    /* 4 */ 8,
    /* 5 */ 5,
    /* 6 */ 7,
    /* 7 */ 6,
    /* 8 */ 4,
    /* 9 */ 0
};

/* Count numbers in [0, X] with digits in non‑decreasing order of the
 * Slovak digit name ranking.  Uses a digit DP with a 4‑dimensional table. */
static uint64_t count_leq(uint64_t X)
{
    int digits[20];
    int len = 0;

    if (X == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        uint64_t tmp = X;
        while (tmp) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
        /* reverse to most significant digit first */
        for (int i = 0; i < len / 2; ++i) {
            int t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    /* DP table: pos (0..len), lastRank (0..10), tight (0/1), started (0/1) */
    uint64_t dp[21][11][2][2] = {{0}};
    /* Base case: when all positions processed, count 1 (the number itself). */
    for (int last = 0; last <= 10; ++last)
        for (int tight = 0; tight <= 1; ++tight)
            for (int started = 0; started <= 1; ++started)
                dp[len][last][tight][started] = 1ULL;

    /* Fill DP table from the most significant digit downwards. */
    for (int pos = len - 1; pos >= 0; --pos) {
        for (int last = 0; last <= 10; ++last) {
            for (int tight = 0; tight <= 1; ++tight) {
                for (int started = 0; started <= 1; ++started) {
                    uint64_t sum = 0;
                    int lim = (tight ? digits[pos] : 9);
                    for (int d = 0; d <= lim; ++d) {
                        int next_tight = (tight && d == lim) ? 1 : 0;
                        if (!started && d == 0) {
                            /* Still haven't started the number: skip leading zero. */
                            sum += dp[pos + 1][10][next_tight][0];
                        } else {
                            unsigned char r = rank_of[d];
                            if (started && r < last) continue;  /* violates order */
                            sum += dp[pos + 1][r][next_tight][1];
                        }
                    }
                    dp[pos][last][tight][started] = sum;
                }
            }
        }
    }

    return dp[0][10][1][0];
}

/* Main sequence function.  Index n >= 1.  Uses binary search. */
seq_t a(idx_t n)
{
    uint64_t target = (uint64_t)n;  /* n fits in 64‑bit for the range needed */
    uint64_t lo = 0, hi = 1;

    /* Increase hi until the count of numbers ≤ hi is at least n. */
    while (count_leq(hi) < target) {
        if (hi > UINT64_MAX / 2) break;  /* safety for extremely large n */
        hi <<= 1;
    }

    /* Binary search for the minimal X with count ≥ n. */
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (count_leq(mid) >= target)
            hi = mid;
        else
            lo = mid + 1;
    }

    return (seq_t)lo;
}
