/*
 * A392036  Irregular table read by rows: the n-th row contains, in weakly
 *          decreasing order, the powers of 2 summing to n as obtained by the
 *          greedy algorithm.
 * https://oeis.org/A392036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th element in the concatenation of the greedy
 *            binary decompositions of the positive integers
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Return the index (0‑based) of the most significant set bit of x
   (x > 0). */
static inline int msb_idx(idx_t x) {
    int pos = 0;
    while (x >>= 1)
        ++pos;
    return pos;
}

/* Sum of popcounts of all numbers 1..n (inclusive).  Uses 128‑bit
   arithmetic to avoid overflow. */
static inline unsigned __int128 sum_popcount_upto(idx_t n) {
    unsigned __int128 total = 0;
    while (n > 0) {
        int msb = msb_idx(n);           /* msb position */
        idx_t p = ((idx_t)1) << msb;    /* 2^msb */
        if (msb > 0)
            total += (unsigned __int128)msb * ((unsigned __int128)1 << (msb - 1));
        total += (unsigned __int128)(n - p + 1); /* numbers with msb set */
        n -= p;                         /* remaining part */
    }
    return total;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 0, return 0 */

    /* Binary search to find the row r such that the cumulative
       number of entries up to r is at least n. */
    idx_t low = 1, high = n;       /* r is always ≤ n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 s = sum_popcount_upto(mid);
        if (s >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Position inside row r (1‑based). */
    unsigned __int128 prevS = (r > 1) ? sum_popcount_upto(r - 1) : 0;
    idx_t c = (idx_t)(n - prevS);  /* 1 <= c <= popcount(r) */

    /* Determine the exponent of the c‑th largest set bit in r. */
    int pop = __builtin_popcountll(r);
    int pos = pop - (int)c;        /* 0‑based index counting from LSB */

    int exponent = -1;
    int count = 0;
    for (int i = 0; i < 64; ++i) {
        if (r & (((idx_t)1) << i)) {
            if (count == pos) {
                exponent = i;
                break;
            }
            ++count;
        }
    }

    return pow2((idx_t)exponent);
}
