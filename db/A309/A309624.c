/*
 * A309624  Sum of the odd parts in the partitions of n into 7 parts.
 * https://oeis.org/A309624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 41, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*  Sum of the odd parts in the partitions of n into 7 parts.
    Computation: dynamic programming over part sizes.
    For each n the algorithm fills two 2‑D tables:
    cnt[p][s]  - number of partitions of s into exactly p parts,
    sumv[p][s] - sum of odd parts in those partitions.
    Complexity:   O(n²·7)   (≈350 M elementary ops for n = 10 000)
    Memory:       O(n·7)    (≈5 MB for n ≤ 20 000)
*/
#define MAX_N 20001   /* must be ≥ maximum n queried (here 10 000) */

static seq_t cnt[8][MAX_N];
static seq_t sumv[8][MAX_N];

/* Build tables up to n (inclusive) */
static void build_tables(idx_t n)
{
    /* reset tables up to n */
    for (int p = 0; p <= 7; ++p) {
        for (idx_t s = 0; s <= n; ++s) {
            cnt[p][s]  = 0;
            sumv[p][s] = 0;
        }
    }
    cnt[0][0] = 1;   /* one way to partition zero with zero parts */

    for (idx_t m = 1; m <= n; ++m) {
        bool odd = (m & 1ULL) != 0ULL;
        seq_t m_seq = (seq_t)m;
        for (int parts = 1; parts <= 7; ++parts) {
            for (idx_t s = m; s <= n; ++s) {
                seq_t old_cnt = cnt[parts - 1][s - m];
                if (old_cnt == 0) continue;
                /* count of partitions */
                cnt[parts][s] += old_cnt;
                /* sum of odd parts */
                seq_t tmp = sumv[parts - 1][s - m];
                if (odd) tmp += old_cnt * m_seq;
                sumv[parts][s] += tmp;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n >= MAX_N) {
        /* n too large for the fixed table size – trigger a safe abort */
        return 0;
    }
    build_tables(n);
    return sumv[7][n];
}
