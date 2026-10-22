/*
 * A338156  Irregular triangle read by rows in which row n lists n blocks,
 *          where the m-th block consists of A000041(m-1) copies of the
 *          divisors of (n - m + 1), with 1 <= m <= n.
 * https://oeis.org/A338156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9430, u16 n <= 9430, u32 n <= 9430, u64 n <= 9430,
 *            u128 n <= 9430, big any n
 * Helpers:   divisor_at_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes A338156: irregular triangle read by rows.
 * Row n contains n blocks; the m‑th block is A000041(m-1) copies
 * of the sorted divisors of (n‑m+1). This function returns the
 * n‑th term when the triangle is read row by row.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/divisor_at_u64.h"

/* maximum k for which we need partition numbers */
#define PART_MAX 2000

/* partition numbers stored as 128 bit integers to avoid overflow */
static unsigned __int128 part_tab[PART_MAX + 1];
static bool part_init = false;

/* initialise partition numbers up to PART_MAX */
static void init_partitions(void)
{
    if (part_init) return;
    part_tab[0] = 1;
    for (size_t i = 1; i <= PART_MAX; ++i) part_tab[i] = 0;
    for (size_t i = 1; i <= PART_MAX; ++i) {
        for (size_t j = i; j <= PART_MAX; ++j) {
            part_tab[j] += part_tab[j - i];
        }
    }
    part_init = true;
}

/* number of divisors of n (unordered) */
static uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t cnt = 0;
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            ++cnt;
            if (d * d != n) ++cnt;
        }
    }
    return cnt;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* protection for offset 1 */

    init_partitions();

    unsigned __int128 target = (unsigned __int128)n;
    unsigned __int128 cumulative = 0;

    for (idx_t row = 1; ; ++row) {
        /* size of this row: sum_{m=1..row} p(m-1) * d(row-m+1) */
        unsigned __int128 row_size = 0;
        for (idx_t m = 1; m <= row; ++m) {
            unsigned __int128 part = part_tab[m - 1];
            uint64_t val = (uint64_t)(row - m + 1);
            uint64_t dcnt = divisor_count_u64(val);
            row_size += part * (unsigned __int128)dcnt;
        }
        if (cumulative + row_size >= target) {
            /* the term lies in this row */
            unsigned __int128 offset = target - cumulative - 1; /* 0‑based */
            unsigned __int128 block_offset = offset;
            for (idx_t m = 1; m <= row; ++m) {
                unsigned __int128 part = part_tab[m - 1];
                uint64_t val = (uint64_t)(row - m + 1);
                uint64_t dcnt = divisor_count_u64(val);
                unsigned __int128 block_size = part * (unsigned __int128)dcnt;
                if (block_offset < block_size) {
                    /* found the block */
                    uint64_t div_idx = (uint64_t)(block_offset % dcnt);
                    return divisor_at_u64(val, div_idx);
                }
                block_offset -= block_size;
            }
            /* should never reach here */
            return 0;
        }
        cumulative += row_size;
    }
}
