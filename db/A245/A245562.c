/*
 * A245562  Table read by rows: row n gives list of lengths of runs of 1's in
 *          binary expansion of n, starting with high-order bits.
 * https://oeis.org/A245562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, easy, changed
 * Formula:   a(n) is the nth element of the flattened table formed by writing
 *            the lengths of consecutive 1‑runs in the binary expansion of
 *            each integer, in ascending order starting with the empty list
 *            for 0. The first element (n = 0) is a sentinel 0
 * Range:     u8 n <= 2770, u16 n <= 2770, u32 n <= 2770, u64 n <= 2770,
 *            u128 n <= 2770, big any n
 * Helpers:   count_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_runs.h"

static inline uint64_t bitlen(uint64_t x)
{
    return x ? 64 - __builtin_clzll(x) : 0;
}

static inline uint64_t run_len_at(uint64_t x, uint64_t pos)
{
    /* returns the (pos)-th run length (0‑based) in the run list of x,
       where runs are counted from the most significant bit. */
    if (x == 0) return 0;
    bool prev_one = false;
    uint64_t run_idx = 0;
    uint64_t run_len = 0;
    uint64_t bl = bitlen(x);
    for (int i = (int)bl - 1; i >= 0; --i)
    {
        bool is_one = (x >> i) & 1ull;
        if (is_one)
        {
            if (!prev_one)
            {
                /* start of a new run */
                if (run_idx == pos) run_len = 1;
                run_idx++;
            }
            else
            {
                if (run_idx - 1 == pos) run_len++;
            }
        }
        prev_one = is_one;
    }
    return run_len;
}

seq_t a(idx_t n)
{
    /* a(0) = 0 sentinel */
    if (n == 0) return 0;

    idx_t target = n;
    idx_t cumulative = 1;   /* count after including the sentinel at index 0 */
    uint64_t m = 1;

    while (1)
    {
        uint64_t len = count_runs(m);
        if (cumulative + len > target)
        {
            idx_t pos = target - cumulative;  /* position within row m */
            return (seq_t)run_len_at(m, (uint64_t)pos);
        }
        cumulative += len;
        m++;
    }
}
