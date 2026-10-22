/*
 * A358097  a(n) is the smallest integer m > n such that m and n have no
 *          common digit, or -1 when such integer m does not exist.
 * https://oeis.org/A358097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the smallest integer m > n such that * m and n have no
 *            common decimal digit, or -1 if such m * does not exist. The
 *            digits of m must be chosen from the * complement set of digits
 *            appearing in n. If n uses all * digits 1–9 (regardless of 0) or
 *            all digits 0–9, the result * is -1. * The funct...
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Recursive helper to find minimal allowed number of the same length
 * that is strictly larger than n.
 */
static seq_t dfs(int pos, bool greater, seq_t cur,
                 int len, const int *n_digits,
                 const int *allow_all, int sz_all,
                 const int *allow_non, int sz_non)
{
    if (pos == len)
        return greater ? cur : (seq_t)-1;

    const int *opts = (pos == 0) ? allow_non : allow_all;
    int sz = (pos == 0) ? sz_non : sz_all;

    for (int i = 0; i < sz; ++i) {
        int d = opts[i];
        if (!greater && d < n_digits[pos])
            continue;
        bool next_greater = greater || (d > n_digits[pos]);
        seq_t res = dfs(pos + 1, next_greater, cur * 10 + d,
                        len, n_digits, allow_all, sz_all,
                        allow_non, sz_non);
        if (res != (seq_t)-1)
            return res;
    }
    return (seq_t)-1;
}

seq_t a(idx_t n)
{
    /* 1. Compute digit mask of n. */
    int mask = 0;
    uint64_t t = (uint64_t)n;
    if (t == 0) mask |= 1 << 0;
    while (t > 0) {
        mask |= 1 << (t % 10);
        t /= 10;
    }

    /* 2. Quick check for impossible cases. */
    if (mask == 0x3FF)                /* all digits 0–9 used     */
        return (seq_t)-1;
    if ((mask & 0x3FE) == 0x3FE)     /* digits 1–9 all used    */
        return (seq_t)-1;

    /* 3. Build allowed digit lists (ascending). */
    int allow_all[10], sz_all = 0;
    int allow_non[10], sz_non = 0;
    for (int d = 0; d <= 9; ++d) {
        if (!(mask & (1 << d))) {
            allow_all[sz_all++] = d;
            if (d != 0)
                allow_non[sz_non++] = d;
        }
    }
    if (sz_all == 0)  /* no digits available at all */
        return (seq_t)-1;

    /* 4. Extract digits of n in most‑significant order. */
    int msd[20], len = 0;
    if (n == 0) {
        msd[0] = 0;
        len = 1;
    } else {
        int rev[20], rlen = 0;
        uint64_t tt = (uint64_t)n;
        while (tt > 0) {
            rev[rlen++] = tt % 10;
            tt /= 10;
        }
        for (int i = 0; i < rlen; ++i)
            msd[i] = rev[rlen - 1 - i];
        len = rlen;
    }

    /* 5. Search for minimal allowed number of the same length. */
    seq_t res = dfs(0, false, 0, len, msd,
                    allow_all, sz_all, allow_non, sz_non);
    if (res != (seq_t)-1)
        return res;

    /* 6. Construct minimal number with one additional digit. */
    if (sz_non == 0)   /* no non‑zero allowed digit for leading place */
        return (seq_t)-1;

    seq_t val = 0;
    val = val * 10 + (seq_t)allow_non[0];
    for (int i = 0; i < len; ++i)
        val = val * 10 + (seq_t)allow_all[0];
    return val;
}
