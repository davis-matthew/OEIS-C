/*
 * A382725  Number of entries in the n-th row of Pascal's triangle not
 *          divisible by 8.
 * https://oeis.org/A382725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{k | 0 <= k <= n, popcount(k) + popcount(n-k) -
 *            popcount(n) < 3}|
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here popcount(x) is the number of 1‑bits in the binary representation of x. */

#include <stdint.h>

#define MAX_SUM 70   /* > maximum popcount(n)+3 for 64‑bit n (64+3=67) */

seq_t a(idx_t n)
{
    /* Count binary digits (number of bits in n) */
    int bits = 0;
    uint64_t tmp = (uint64_t)n;
    while (tmp) { bits++; tmp >>= 1; }

    /* popcount of n using GCC built‑in */
    int pop_n = __builtin_popcountll((uint64_t)n);
    int max_sum = pop_n + 3;           /* we only need sums < max_sum */

    /* dynamic programming arrays: cur[borrow][sum] */
    seq_t cur[2][MAX_SUM] = {0}, nxt[2][MAX_SUM];
    cur[0][0] = 1;                     /* empty lower bits: no carries, zero sum */

    for (int pos = 0; pos < bits; ++pos) {
        /* clear next state array */
        for (int b = 0; b < 2; ++b)
            for (int s = 0; s < max_sum; ++s)
                nxt[b][s] = 0;

        int nb = ( (uint64_t)n >> pos ) & 1;  /* current bit of n */

        for (int bprev = 0; bprev < 2; ++bprev) {
            for (int s = 0; s < max_sum; ++s) {
                seq_t cnt = cur[bprev][s];
                if (!cnt) continue;

                for (int kbit = 0; kbit <= 1; ++kbit) {
                    int borrow_out = (kbit + bprev > nb) ? 1 : 0;
                    int di = nb - kbit - bprev;
                    if (di < 0) di += 2;                /* bit of (n-k) */
                    int snew = s + kbit + di;
                    if (snew < max_sum)
                        nxt[borrow_out][snew] += cnt;
                }
            }
        }
        /* copy next state to current for next iteration */
        for (int b = 0; b < 2; ++b)
            for (int s = 0; s < max_sum; ++s)
                cur[b][s] = nxt[b][s];
    }

    /* Sum all valid k's: final borrow must be 0 */
    seq_t result = 0;
    for (int s = 0; s < max_sum; ++s)
        result += cur[0][s];

    return result;
}
