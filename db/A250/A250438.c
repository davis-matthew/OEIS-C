/*
 * A250438  Number of (n+1)X(3+1) 0..2 arrays with nondecreasing sum of every
 *          two consecutive values in every row and column.
 * https://oeis.org/A250438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 54, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A250438
    Number of (n+1)×(3+1) 0..2 arrays with nondecreasing sum of every
    two consecutive values in every row and column.
    Formula used:
        a(n) = ( C(ceil((n+1)/2)) * C(floor((n+1)/2)) )^2
    where C(m) = number of length‑m chains in the poset
                {(x,y) : 0 ≤ x ≤ y ≤ 2} ordered component‑wise.
    The chain count C(m) is computed by a small DP of 6 states.
*/

#include <stdint.h>

/* Transition matrix for the poset {(x,y)}:
   transition[i][j] == 1 iff element i <= element j component‑wise. */
static const uint8_t transition[6][6] = {
    /* 00 */ {1,1,1,1,1,1},
    /* 01 */ {0,1,1,1,1,1},
    /* 02 */ {0,0,1,0,1,1},
    /* 11 */ {0,0,0,1,1,1},
    /* 12 */ {0,0,0,0,1,1},
    /* 22 */ {0,0,0,0,0,1}
};

/* Computes C(m): number of chains of length m in the above poset. */
static seq_t
chain_count(idx_t m)
{
    seq_t dp[6];
    /* base: length 1, each element can start a chain. */
    for (int i = 0; i < 6; ++i)
        dp[i] = 1;

    /* length 2 .. m */
    for (idx_t step = 2; step <= m; ++step) {
        seq_t ndp[6] = {0};
        for (int j = 0; j < 6; ++j) {
            seq_t sum = 0;
            for (int i = 0; i < 6; ++i)
                if (transition[i][j])
                    sum += dp[i];
            ndp[j] = sum;
        }
        for (int i = 0; i < 6; ++i)
            dp[i] = ndp[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 6; ++i)
        total += dp[i];
    return total;
}

/* Public interface required by the platform. */
seq_t a(idx_t n)
{
    /* rows = n + 1 */
    idx_t m_even = (n + 2) / 2;  /* number of even‑indexed rows */
    idx_t m_odd  = (n + 1) / 2;  /* number of odd‑indexed rows */

    seq_t even_cnt = chain_count(m_even);
    seq_t odd_cnt  = chain_count(m_odd);

    seq_t prod = even_cnt * odd_cnt;
    return prod * prod;          /* square it to get the final count */
}
