/*
 * A301453  a(n) is the number of ways of writing the binary expansion of n as
 *          a concatenation of nonempty substrings with no two consecutive
 *          equal substrings.
 * https://oeis.org/A301453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is computed by counting all partitions of the * binary
 *            expansion of n into non‑empty substrings with no two *
 *            consecutive equal substrings. The algorithm is a memoized depth
 *            *‑first search over the positions of the string; the number of *
 *            states is O(L·2^L) with L the bit‑length of n (L ≤ 14...
 * Range:     u8 n <= 516, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 8959 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t dfs_helper(int pos, int prev_start, int L,
                        const seq_t (*val)[65], seq_t (*memo)[66])
{
    int ps_index = prev_start + 1;            /* prev_start == -1 mapped to 0 */
    if (memo[pos][ps_index] != 0)
        return memo[pos][ps_index];

    if (pos == L)
        return memo[pos][ps_index] = 1;       /* empty suffix → one way */

    seq_t total = 0;
    for (int j = pos; j < L; ++j) {
        int cur_len = j - pos + 1;
        if (prev_start == -1) {
            /* first substring, no restriction */
            total += dfs_helper(j + 1, pos, L, val, memo);
        } else {
            int prev_len = pos - prev_start;
            if (cur_len != prev_len ||
                val[prev_start][pos - 1] != val[pos][j]) {
                total += dfs_helper(j + 1, pos, L, val, memo);
            }
        }
    }
    memo[pos][ps_index] = total;
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                               /* empty string */

    /* convert n to binary string, MSB first */
    int bits[65] = {0};
    int L = 0;
    uint64_t tmp = n;
    while (tmp > 0) {
        bits[L++] = (int)(tmp & 1);
        tmp >>= 1;
    }
    int bin[65] = {0};
    for (int i = 0; i < L; ++i)
        bin[i] = bits[L - 1 - i];                /* MSB first */

    /* pre‑compute values of all substrings */
    seq_t val[65][65] = {0};
    for (int i = 0; i < L; ++i) {
        val[i][i] = (seq_t)bin[i];
        for (int j = i + 1; j < L; ++j) {
            val[i][j] = (val[i][j - 1] << 1) + (seq_t)bin[j];
        }
    }

    /* memoization table: pos 0..L, prev_start+1 0..L */
    seq_t memo[65][66] = {0};

    return dfs_helper(0, -1, L, val, memo);
}
