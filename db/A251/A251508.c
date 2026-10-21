/*
 * A251508  Number of (n+1)X(1+1) 0..4 arrays with every 2X2 subblock summing
 *          to a prime.
 * https://oeis.org/A251508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1ᵀ * Mⁿ * 1, where M[i][j] = 1 if the sum of all four *
 *            digits of row pair (i, j) is a prime (2, 3, 5, 7, 11, 13). This
 *            counts * (n+1)×2 arrays over {0..4} with every 2×2 subblock
 *            summing to a prime
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define NUM_STATES 25

/* encode the 25 possible rows as two digits in {0..4} */
static const unsigned char state_digits[NUM_STATES][2] = {
    {0,0},{0,1},{0,2},{0,3},{0,4},
    {1,0},{1,1},{1,2},{1,3},{1,4},
    {2,0},{2,1},{2,2},{2,3},{2,4},
    {3,0},{3,1},{3,2},{3,3},{3,4},
    {4,0},{4,1},{4,2},{4,3},{4,4}
};

/* adjacency[i][j] == true iff the sum of the four digits of rows i and j is prime */
static bool adjacency[NUM_STATES][NUM_STATES];
static bool adjacency_ready = false;

static void init_adjacency(void)
{
    if (adjacency_ready) return;

    const int primes[] = {2,3,5,7,11,13};
    const int num_primes = 6;

    for (unsigned i = 0; i < NUM_STATES; ++i) {
        for (unsigned j = 0; j < NUM_STATES; ++j) {
            int sum = state_digits[i][0] + state_digits[i][1]
                    + state_digits[j][0] + state_digits[j][1];
            bool ok = false;
            for (int p = 0; p < num_primes; ++p)
                if (sum == primes[p]) { ok = true; break; }
            adjacency[i][j] = ok;
        }
    }
    adjacency_ready = true;
}

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1. */
    init_adjacency();

    seq_t dp[NUM_STATES];
    seq_t newdp[NUM_STATES];

    /* Start with sequences of length 1 (a single row). */
    for (unsigned i = 0; i < NUM_STATES; ++i) dp[i] = 1;

    for (idx_t step = 0; step < n; ++step) {
        for (unsigned j = 0; j < NUM_STATES; ++j) newdp[j] = 0;

        for (unsigned i = 0; i < NUM_STATES; ++i) {
            seq_t v = dp[i];
            if (v == 0) continue;
            for (unsigned j = 0; j < NUM_STATES; ++j) {
                if (adjacency[i][j]) newdp[j] += v;
            }
        }
        for (unsigned j = 0; j < NUM_STATES; ++j) dp[j] = newdp[j];
    }

    seq_t total = 0;
    for (unsigned j = 0; j < NUM_STATES; ++j) total += dp[j];
    return total;
}
