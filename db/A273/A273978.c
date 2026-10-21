/*
 * A273978  List of words of length n over an alphabet of size 9 that are in
 *          standard order and which have the property that every letter that
 *          appears in the word is repeated.
 * https://oeis.org/A273978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 4360, u64 n <= 4360,
 *            u128 n <= 4360, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: enumerates words of length n over alphabet {1..9} in standard order, with each letter repeated; returns the nth word as an integer. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define RMAX   200      /* maximum word length considered for n up to ~10⁴            */
#define ALPHA  9       /* alphabet size */

static seq_t dp[RMAX][ALPHA+1][1 << ALPHA];
static bool vis[RMAX][ALPHA+1][1 << ALPHA];

/* Count how many completions exist for the remaining positions.
   rem  : remaining positions to fill
   k    : largest letter used so far
   mask : bits i set indicate that letter (i+1) still needs one more occurrence */
static seq_t count_completions(int rem, int k, int mask) {
    if (rem == 0) {
        return (mask == 0) ? (seq_t)1 : (seq_t)0;
    }
    if (vis[rem][k][mask]) return dp[rem][k][mask];
    vis[rem][k][mask] = true;
    seq_t total = 0;

    /* Place an already used letter. */
    for (int d = 1; d <= k; ++d) {
        int newmask = mask;
        if (mask & (1 << (d-1))) newmask &= ~(1 << (d-1));
        total += count_completions(rem-1, k, newmask);
    }

    /* Introduce the new letter k+1, if it can still be completed. */
    if (k < ALPHA && rem-1 >= 1) {
        int newmask = mask | (1 << k);          /* first occurrence of k+1 */
        total += count_completions(rem-1, k+1, newmask);
    }

    dp[rem][k][mask] = total;
    return total;
}

/* Number of valid words of exact length L (L>=2). */
static seq_t total_words_of_length(int L) {
    if (L < 2) return (seq_t)0;
    return count_completions(L-1, 1, 1);          /* first letter is 1    */
}

/* Cumulative counts: cum[L] = number of words of length <= L. */
static seq_t cum[RMAX+1];
static size_t cum_len = 1;          /* cum[1] is already defined (0) */

/* Ensure cumulative counts known up to the smallest L with cum[L] >= n. */
static void ensure_cumulative(idx_t n) {
    if (cum_len < 2) {              /* initialise cum[0] and cum[1] */
        cum[0] = 0;
        cum[1] = 0;
        cum_len = 1;
    }
    while (cum_len < (size_t)RMAX && cum[cum_len] < n) {
        ++cum_len;
        seq_t cnt = total_words_of_length((int)cum_len);
        cum[cum_len] = cum[cum_len-1] + cnt;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;                 /* undefined for indices < 1 */

    ensure_cumulative(n);

    /* Find the word length L such that a(n) has length L. */
    int L = 2;
    while (L <= RMAX && cum[L] < n) ++L;
    if ((size_t)L > RMAX) return (seq_t)0;              /* beyond pre‑computed range */

    seq_t idx_into_length = n - cum[L-1];   /* 1‑based index within length L */

    /* Construct the word digit by digit. */
    seq_t word = 1;                       /* first letter fixed to 1   */
    int r = L-1;                         /* remaining positions       */
    int k = 1;                           /* current maximum letter    */
    int mask = 1;                        /* letter 1 still needs copy */

    for (int pos = 2; pos <= L; ++pos) {
        for (int d = 1; d <= ALPHA; ++d) {
            if (d > k+1) break;          /* cannot skip any letter   */
            seq_t sub = 0;
            int newk = k;
            int newmask = mask;

            if (d <= k) {                /* existing letter          */
                if (mask & (1 << (d-1))) newmask &= ~(1 << (d-1));
                sub = count_completions(r-1, k, newmask);
            } else {                    /* introduce new letter d=k+1 */
                if (r-1 < 1) continue;    /* insufficient space for its second copy */
                newk = k+1;
                newmask = mask | (1 << k);
                sub = count_completions(r-1, newk, newmask);
            }

            if (sub < idx_into_length) {
                idx_into_length -= sub;
                continue;                /* try next digit           */
            }

            /* Choose this digit. */
            word = word * (seq_t)10 + (seq_t)d;
            r   -= 1;
            k    = newk;
            mask = newmask;
            break;
        }
    }

    return word;
}
