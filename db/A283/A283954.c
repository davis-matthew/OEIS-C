/*
 * A283954  Number of 5Xn 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary arrays in which all 1’s have at most
 *            three horizontal, diagonal, or anti‑diagonal neighbours, except
 *            for exactly one 1 which may have more than three neighbours
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Transition table: trans[prev][curr][next] is  
   -1  if the configuration causes >1 violating 1’s in the column `curr`
    0  if it causes no violations
    1  if it causes exactly one violation
  The transition table is pre‑computed once.                                     */
/* ------------------------------------------------------------------ */
static bool ready = false;
static int8_t trans[32][32][32];

static void init_trans(void) {
    if (ready) return;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            for (int next = 0; next < 32; ++next) {
                int viol = 0;
                for (int r = 0; r < 5; ++r) {
                    if (!((curr >> r) & 1)) continue;   /* only 1’s matter */
                    int nb = 0;
                    /* horizontal neighbours */
                    if ((prev >> r) & 1) ++nb;          /* left */
                    if ((next >> r) & 1) ++nb;          /* right */
                    /* diagonal to left */
                    if (r > 0      && (prev >> (r - 1)) & 1) ++nb;   /* up‑left */
                    if (r < 4      && (prev >> (r + 1)) & 1) ++nb;   /* down‑left */
                    /* diagonal to right */
                    if (r > 0      && (next >> (r - 1)) & 1) ++nb;   /* up‑right */
                    if (r < 4      && (next >> (r + 1)) & 1) ++nb;   /* down‑right */
                    if (nb > 3) {
                        ++viol;
                        if (viol > 1) break;
                    }
                }
                if (viol == 0)          trans[prev][curr][next] = 0;
                else if (viol == 1)     trans[prev][curr][next] = 1;
                else                    trans[prev][curr][next] = -1;  /* >1 violations */
            }
        }
    }
    ready = true;
}

/* ------------------------------------------------------------------ */
/* Main counting routine                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    init_trans();

    /* DP: state index = ((prev << 5) | curr) << 1 | used   (used = 0/1) */
    enum { STATES = 2048 };            /* 32 * 32 * 2 */
    static seq_t dp[STATES];
    static seq_t nxt[STATES];

    for (size_t i = 0; i < STATES; ++i) dp[i] = 0;
    dp[0] = 1;                                 /* prev=0, curr=0, used=0 */

    for (idx_t col = 0; col < n; ++col) {
        for (size_t i = 0; i < STATES; ++i) nxt[i] = 0;
        for (size_t s = 0; s < STATES; ++s) {
            seq_t ways = dp[s];
            if (ways == 0) continue;
            int used = s & 1;
            size_t t = s >> 1;
            int curr = t & 31;
            int prev = t >> 5;

            for (int next = 0; next < 32; ++next) {
                int8_t add = trans[prev][curr][next];
                if (add < 0) continue;                 /* >1 violations inside curr */
                int new_used = used + add;
                if (new_used > 1) continue;
                size_t ns = (((curr << 5) | next) << 1) | new_used;
                nxt[ns] += ways;
            }
        }
        for (size_t i = 0; i < STATES; ++i) dp[i] = nxt[i];
    }

    /* Final dummy column of zeros to process the last real column */
    seq_t ans = 0;
    for (size_t s = 0; s < STATES; ++s) {
        seq_t ways = dp[s];
        if (ways == 0) continue;
        int used = s & 1;
        size_t t = s >> 1;
        int curr = t & 31;
        int prev = t >> 5;

        int8_t add = trans[prev][curr][0];          /* next = 0 (dummy) */
        if (add < 0) continue;
        int new_used = used + add;
        if (new_used == 1) ans += ways;            /* exactly one violation overall */
    }
    return ans;
}
