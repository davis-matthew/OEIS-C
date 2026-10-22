/*
 * A385816  The number k such that the k-th composition in standard order
 *          lists the maximal anti-run lengths of the binary indices of n.
 *          Standard composition number of row n of A384877.
 * https://oeis.org/A385816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = composition number of the maximal anti‑run lengths of the
 *            binary indices of n (in standard order)
 * Range:     u8 n <= 510, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n = 0 has no binary 1's → empty composition → rank 0 */
    if (n == 0)
        return 0;

    /* First collect the 1‑bit positions (1‑based). */
    int idx[64];       /* maximum 64 bits */
    int idx_cnt = 0;

    idx_t t = n;
    int bit_pos = 0;
    while (t != 0) {
        if (t & (idx_t)1)
            idx[idx_cnt++] = bit_pos + 1;   /* 1‑based index */
        t >>= 1;
        ++bit_pos;
    }

    /* Compute the anti‑runs: maximal subsequences where consecutive
       indices differ by not exactly 1. */
    int runs[64];
    int runs_cnt = 0;
    int cur_len = 0;
    int prev = -100;          /* sentinel far smaller than any real index */

    for (int i = 0; i < idx_cnt; ++i) {
        int cur = idx[i];
        if (cur_len == 0) {           /* start first run */
            cur_len = 1;
        } else {
            if (cur - prev == 1) {    /* boundary – split run */
                runs[runs_cnt++] = cur_len;
                cur_len = 1;
            } else {                  /* extend current run */
                ++cur_len;
            }
        }
        prev = cur;
    }
    if (cur_len > 0)
        runs[runs_cnt++] = cur_len;

    /* Cuts occur after each run except the last one:
       cumulative sums of run lengths. */
    int cuts[64];
    int cuts_cnt = 0;
    int cum = 0;
    for (int i = 0; i < runs_cnt - 1; ++i) {
        cum += runs[i];
        cuts[cuts_cnt++] = cum;   /* position of a cut (1‑based) */
    }

    /* Weight = number of 1‑bits = sum of run lengths = idx_cnt. */
    int m = idx_cnt;               /* 1 ≤ m ≤ 64 */

    /* Compute rank r within all compositions of weight m.
       The rank equals the integer value of the binary string
       of length m-1, where a 1 bit marks a cut. */
    seq_t r = 0;
    int cut_i = 0;
    for (int pos = 1; pos <= m - 1; ++pos) {
        r <<= 1;                     /* shift left for next bit */
        if (cut_i < cuts_cnt && cuts[cut_i] == pos) {
            r |= (seq_t)1;           /* set bit for a cut position */
            ++cut_i;
        }
    }

    /* Compute 2^(m-1) safely, even for tiny seq_t types. */
    seq_t pow2 = 1;
    for (int i = 0; i < m - 1; ++i)
        pow2 <<= 1;

    /* k = 2^(m-1) + r  (for m > 0).  Empty composition handled above. */
    return pow2 + r;
}
