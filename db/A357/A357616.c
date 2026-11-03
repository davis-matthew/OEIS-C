/*
 * A357616  Lexicographically earliest sequence of distinct nonnegative
 *          integers such that for any n >= 0, the number of 1's in the
 *          ternary expansion of n equals the number of 2's in the ternary
 *          expansion of a(n) and vice versa.
 * https://oeis.org/A357616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the lexicographically earliest permutation of the
 *            nonnegative integers such that #1's in n = #2's in a(n) and vice
 *            versa
 * Range:     u8 n <= 242, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Validated: standard, pass, 84 terms through n = 5994 (2026-11-01)
 * Helpers:   binomial_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard integer types are provided elsewhere */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial_u64.h"

/* Count of suffix sequences of length L (leading zeros allowed)
   containing exactly c1 digits '1' and c2 digits '2'. */
static inline uint64_t _count_suffix(int L, int c1, int c2) {
    if (L < 0 || (c1 < 0) || (c2 < 0) || (c1 + c2 > L))
        return 0;
    return binomial_u64(L, c1) * binomial_u64(L - c1, c2);
}

/* Count of ternary numbers of length L (leading digit cannot be 0)
   containing exactly c1 digits '1' and c2 digits '2'. */
static inline uint64_t _count_total(int L, int c1, int c2) {
    if (L <= 0 || (c1 < 0) || (c2 < 0) || (c1 + c2 > L))
        return 0;
    uint64_t cnt = 0;
    if (c1 > 0) {
        /* leading digit = 1 */
        cnt += binomial_u64(L - 1, c1 - 1) * binomial_u64(L - 1 - (c1 - 1), c2);
    }
    if (c2 > 0) {
        /* leading digit = 2 */
        cnt += binomial_u64(L - 1, c1) * binomial_u64(L - 1 - c1, c2 - 1);
    }
    return cnt;
}

/* Reconstruct the number of rank r (0‑based) in the set
   of numbers with exactly c1 '1's and c2 '2's. */
static inline seq_t _build_from_rank(uint64_t r, int c1, int c2) {
    if (c1 == 0 && c2 == 0) return (seq_t)0;  /* only 0 exists */
    int len_min = (c1 == 0 && c2 == 0) ? 1 : c1 + c2;
    int L = len_min;
    uint64_t cnt = _count_total(L, c1, c2);
    while (r >= cnt) {
        r -= cnt;
        ++L;
        cnt = _count_total(L, c1, c2);
    }

    int rem1 = c1, rem2 = c2;
    seq_t value = 0;
    for (int pos = 0; pos < L; ++pos) {
        int chosen = -1;
        for (int d = 0; d <= 2; ++d) {
            if (pos == 0 && d == 0) continue;            /* no leading 0 */
            if (d == 1 && rem1 == 0) continue;
            if (d == 2 && rem2 == 0) continue;
            int rem1p = rem1, rem2p = rem2;
            if (d == 1) rem1p--;
            else if (d == 2) rem2p--;

            int remaining = L - 1 - pos;
            uint64_t cntp = _count_suffix(remaining, rem1p, rem2p);
            if (r < cntp) {
                chosen = d;
                rem1 = rem1p;
                rem2 = rem2p;
                break;
            } else {
                r -= cntp;
            }
        }
        if (chosen == -1) chosen = 0;                      /* safety */
        value = value * 3 + (seq_t)chosen;
    }
    return value;
}

/* Rank (0‑based) of n inside the set of numbers having
   the same numbers of 1's and 2's as n. */
static inline uint64_t _rank_set(idx_t n, int c1, int c2) {
    if (n == 0) return 0;                /* handled separately */
    /* ternary digits of n, least significant first */
    int digits[64];
    int len = 0;
    idx_t t = n;
    while (t > 0) {
        digits[len++] = t % 3;
        t /= 3;
    }
    int len_min = (c1 == 0 && c2 == 0) ? 1 : c1 + c2;
    uint64_t rank = 0;
    /* lengths strictly smaller than len */
    for (int L = len_min; L < len; ++L)
        rank += _count_total(L, c1, c2);

    int rem1 = c1, rem2 = c2;
    for (int pos = len - 1; pos >= 0; --pos) {
        int cur = digits[pos];
        for (int d = 0; d < cur; ++d) {
            if (pos == len - 1 && d == 0) continue;     /* no leading 0 */
            if (d == 1 && rem1 == 0) continue;
            if (d == 2 && rem2 == 0) continue;
            int rem1p = rem1, rem2p = rem2;
            if (d == 1) rem1p--;
            else if (d == 2) rem2p--;
            int remaining = pos;                         /* digits to the right */
            rank += _count_suffix(remaining, rem1p, rem2p);
        }
        /* consume the actual digit */
        if (cur == 1) rem1--;
        else if (cur == 2) rem2--;
    }
    return rank;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* Count ones and twos in ternary representation of n */
    int c1 = 0, c2 = 0;
    idx_t t = n;
    while (t > 0) {
        int d = t % 3;
        if (d == 1) c1++;
        else if (d == 2) c2++;
        t /= 3;
    }

    /* Compute the rank of n among numbers having (c1,c2) */
    uint64_t rank = _rank_set(n, c1, c2);

    /* Find the number with the same rank among numbers with (c2,c1) */
    seq_t res = _build_from_rank(rank, c2, c1);
    return res;
}
