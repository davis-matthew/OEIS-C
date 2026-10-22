/*
 * A394752  Indices of zeros (P-positions) in the Sprague-Grundy sequence for
 *          the octal game .404.
 * https://oeis.org/A394752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the nth index (offset 1) where the Sprague‑Grundy number
 *            G(n) for the octal game .404 equals 0. G is defined by G(0)=0
 *            and, for n≥1, G(n)=mex{ G(a)^G(b) : a+b=n−1, a,b≥1 } ∪ {
 *            G(a)^G(b) : a+b=n−3, a,b≥1 }. The function computes G
 *            sequentially, storing results in static arrays so subsequen...
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation notes:
   * Grundy numbers are kept in a dynamic array G; zeros indices are kept
     separately in zeroPos.
   * For each new position i>0 we compute G(i) by enumerating all pairs
     (a,b) with a+b=i−1 or i−3 (a,b≥1) and recording the XORs to find
     the mex.  This is O(i) work, so the total cost up to N is O(N²),
     which easily meets the time limits for the sizes encountered in
     the tests. */
/* Includes */
#include <stdlib.h>
#include <stdbool.h>

/* static storage for computed values */
static seq_t *G = NULL;          /* Grundy array */
static size_t G_cap = 0;         /* allocated size for G */
static idx_t G_next = 0;         /* next index to compute */

static idx_t *zeroPos = NULL;    /* indices where G==0 */
static size_t zero_cap = 0;      /* allocated size for zeroPos */
static size_t zero_len = 0;      /* number of zeros found */

static bool *seen = NULL;        /* temporary array for mex calculation */
#define SEEN_CAP 8192          /* assumed sufficient for XOR values */

/* ensure G has room for at least 'need' elements */
static void ensure_G_cap(idx_t need) {
    if (need <= G_cap) return;
    size_t new_cap = G_cap ? G_cap : 1;
    while (new_cap <= need) new_cap <<= 1;
    G = (seq_t *)realloc(G, new_cap * sizeof(seq_t));
    G_cap = new_cap;
}

/* ensure zeroPos has room for at least 'need' elements */
static void ensure_zero_cap(size_t need) {
    if (need <= zero_cap) return;
    size_t new_cap = zero_cap ? zero_cap : 1;
    while (new_cap <= need) new_cap <<= 1;
    zeroPos = (idx_t *)realloc(zeroPos, new_cap * sizeof(idx_t));
    zero_cap = new_cap;
}

/* compute G at index i, assumes G[0..i-1] are already filled */
static seq_t compute_G_at(idx_t i) {
    int max_seen = -1;   /* largest value stored in seen */

    /* sum i-1 */
    if (i >= 1) {
        idx_t s = i - 1;
        if (s >= 2) {
            for (idx_t a = 1; a < s; ++a) {
                idx_t b = s - a;
                seq_t v = G[a] ^ G[b];
                unsigned int idx = (unsigned int)v;
                if (idx < SEEN_CAP) {
                    if (!seen[idx]) {
                        seen[idx] = true;
                        if ((int)idx > max_seen) max_seen = (int)idx;
                    }
                }
            }
        }
    }

    /* sum i-3 */
    if (i >= 3) {
        idx_t s = i - 3;
        if (s >= 2) {
            for (idx_t a = 1; a < s; ++a) {
                idx_t b = s - a;
                seq_t v = G[a] ^ G[b];
                unsigned int idx = (unsigned int)v;
                if (idx < SEEN_CAP) {
                    if (!seen[idx]) {
                        seen[idx] = true;
                        if ((int)idx > max_seen) max_seen = (int)idx;
                    }
                }
            }
        }
    }

    /* find mex */
    seq_t mex = 0;
    while (true) {
        unsigned int idx = (unsigned int)mex;
        if (idx >= SEEN_CAP || !seen[idx]) break;
        mex++;
    }

    /* reset seen entries used */
    for (int j = 0; j <= max_seen; ++j) seen[j] = false;

    return mex;
}

/* public function: return nth zero index (offset 1) */
seq_t a(idx_t n) {
    /* initialization on first call */
    if (!G) {
        G_cap = 1;
        G = (seq_t *)malloc(G_cap * sizeof(seq_t));
        G[0] = (seq_t)0;
        G_next = 1;                /* next Grundy index to compute */

        zero_cap = 1;
        zeroPos = (idx_t *)malloc(zero_cap * sizeof(idx_t));
        zeroPos[0] = 0;            /* G[0] is zero */
        zero_len = 1;

        seen = (bool *)calloc(SEEN_CAP, sizeof(bool));
    }

    /* extend computation until we have n zeros */
    while (zero_len < n) {
        idx_t pos = G_next;
        ensure_G_cap(pos + 1);

        seq_t g_val = compute_G_at(pos);
        G[pos] = g_val;

        if (g_val == 0) {
            ensure_zero_cap(zero_len + 1);
            zeroPos[zero_len++] = pos;
        }

        G_next = pos + 1;
    }

    return (seq_t)zeroPos[n - 1];
}
