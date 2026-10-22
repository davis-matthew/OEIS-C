/*
 * A371343  Lexicographically latest sequence of distinct nonnegative integers
 *          such that for any n >= 0, the binary expansions of n and of a(n)
 *          have the same length (A070939) and the same number of runs of
 *          consecutive equals digits (A005811).
 * https://oeis.org/A371343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the lexicographically latest self‑inverse permutation of
 *            nonnegative integers such that n and a(n) have the same binary
 *            length and the same number of runs of equal bits
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Pre‑compute binomial coefficients C[n][k] for 0 <= n,k <= 64. */
static uint64_t C[65][65];
static bool C_inited = false;
static inline void init_C(void)
{
    if (C_inited) return;
    for (int n = 0; n <= 64; ++n) {
        C[n][0] = C[n][n] = 1ULL;
        for (int k = 1; k < n; ++k) C[n][k] = C[n-1][k-1] + C[n-1][k];
    }
    C_inited = true;
}

/* Count how many binary strings of length len, starting with prev_bit,
   use exactly runs_left runs.  It uses the combinatorial formula:
   number = C[len-1][runs_left] (if first bit equals prev_bit) +
            C[len-1][runs_left-1] (if first bit differs). */
static inline uint64_t count_suffix(unsigned int len,
                                    int runs_left, int prev_bit)
{
    if (runs_left < 0) return 0;
    if (len == 0) return runs_left == 0 ? 1ULL : 0ULL;
    uint64_t res = 0;
    if (runs_left <= (int)(len - 1))
        res += C[len - 1][runs_left];
    if (runs_left >= 1 && runs_left - 1 <= (int)(len - 1))
        res += C[len - 1][runs_left - 1];
    return res;
}

/* Floor log2 for unsigned 64‑bit (n>0). */
static inline int floor_log2_uint64(uint64_t n)
{
    return 63 - __builtin_clzll(n);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    init_C();

    /* Compute binary length L. */
    int L = floor_log2_uint64(n) + 1;            /* L >= 1 */

    /* Count runs of n. */
    int prev_bit = 1;                            /* leading 1 */
    int runs = 1;
    for (int pos = L - 2; pos >= 0; --pos) {
        int bit = (n >> pos) & 1;
        if (bit != prev_bit) {
            ++runs;
            prev_bit = bit;
        }
    }

    /* Size of the group (L, runs). */
    uint64_t group_size = C[L - 1][runs - 1];

    /* Rank of n within the group. */
    int runs_left = runs - 1;                /* runs not yet used after the leading 1 */
    uint64_t rank = 0;
    prev_bit = 1;
    for (int pos = L - 2; pos >= 0; --pos) {
        int bit = (n >> pos) & 1;
        if (bit == 1) {                      /* smaller bit 0 possible */
            int diff = (0 != prev_bit) ? 1 : 0;
            int rem_runs = runs_left - diff;
            if (rem_runs >= 0)
                rank += count_suffix(pos, rem_runs, 0);
        }
        if (bit != prev_bit)
            --runs_left;
        prev_bit = bit;
    }

    /* Mirror rank for the involution. */
    uint64_t mirror_rank = (group_size - 1) - rank;

    /* Unrank to find a(n). */
    seq_t res = ((seq_t)1) << (L - 1);      /* leading 1 bit */
    runs_left = runs - 1;
    prev_bit = 1;
    for (int pos = L - 2; pos >= 0; --pos) {
        unsigned int remain = pos;           /* bits after this position */
        int diff0 = (0 != prev_bit) ? 1 : 0;
        int rem_runs0 = runs_left - diff0;
        uint64_t cnt0 = 0;
        if (rem_runs0 >= 0)
            cnt0 = count_suffix(remain, rem_runs0, 0);

        if (mirror_rank < cnt0) {            /* set bit 0 */
            prev_bit = 0;
            runs_left = rem_runs0;
        } else {                            /* set bit 1 */
            mirror_rank -= cnt0;
            int diff1 = (1 != prev_bit) ? 1 : 0;
            runs_left -= diff1;
            prev_bit = 1;
            res |= ((seq_t)1 << pos);
        }
    }
    return res;
}
