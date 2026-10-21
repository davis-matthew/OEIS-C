/*
 * A296672  Number of nX6 0..1 arrays with each 1 adjacent to 0, 2 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A296672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of n×6 binary arrays in which each 1 is adjacent to
   0, 2, or 3 king‑move neighboring 1s.  This implementation uses
   dynamic programming over triples of rows and works for any seq_t/idx_t
   types supplied by the build. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define WIDTH    6
#define STATES   (1 << (2 * WIDTH))      /* 2^(2*6) = 4096 possible state pairs */

/* For each state (upr,prv) store the list of allowed new rows and the
   number of such rows.  There can be at most 64 transitions per state. */
static unsigned char trans_count[STATES];
static unsigned char trans_patterns[STATES][64];
static bool initialized = false;

/* Test whether a transition from rows (upr,prv) to newr is legal. */
static bool valid_transition(unsigned char upr, unsigned char prv,
                             unsigned char newr)
{
    for (int col = 0; col < WIDTH; ++col) {
        if ((prv >> col) & 1) {
            int cnt = 0;
            /* upper neighbours */
            if (col > 0 && ((upr >> (col - 1)) & 1)) ++cnt;
            if ((upr >> col) & 1)                    ++cnt;
            if (col < WIDTH - 1 && ((upr >> (col + 1)) & 1)) ++cnt;
            /* same row neighbours */
            if (col > 0 && ((prv >> (col - 1)) & 1)) ++cnt;
            if (col < WIDTH - 1 && ((prv >> (col + 1)) & 1)) ++cnt;
            /* lower neighbours */
            if (col > 0 && ((newr >> (col - 1)) & 1)) ++cnt;
            if ((newr >> col) & 1)                    ++cnt;
            if (col < WIDTH - 1 && ((newr >> (col + 1)) & 1)) ++cnt;
            if (!(cnt == 0 || cnt == 2 || cnt == 3)) return false;
        }
    }
    return true;
}

/* Test whether the last (bottom) row satisfies the rule when there
   are no rows below it. */
static bool last_ok(unsigned char upr, unsigned char prv)
{
    for (int col = 0; col < WIDTH; ++col) {
        if ((prv >> col) & 1) {
            int cnt = 0;
            /* upper neighbours */
            if (col > 0 && ((upr >> (col - 1)) & 1)) ++cnt;
            if ((upr >> col) & 1)                    ++cnt;
            if (col < WIDTH - 1 && ((upr >> (col + 1)) & 1)) ++cnt;
            /* same row neighbours */
            if (col > 0 && ((prv >> (col - 1)) & 1)) ++cnt;
            if (col < WIDTH - 1 && ((prv >> (col + 1)) & 1)) ++cnt;
            /* no lower neighbours */
            if (!(cnt == 0 || cnt == 2 || cnt == 3)) return false;
        }
    }
    return true;
}

/* Pre‑compute transition tables.  Done lazily on first call to a(). */
static void init_transitions(void)
{
    const unsigned int mask = (1U << WIDTH) - 1;
    for (unsigned int upr = 0; upr <= mask; ++upr) {
        for (unsigned int prv = 0; prv <= mask; ++prv) {
            unsigned int state = (upr << WIDTH) | prv;
            unsigned char cnt = 0;
            for (unsigned int newr = 0; newr <= mask; ++newr) {
                if (valid_transition((unsigned char)upr,
                                     (unsigned char)prv,
                                     (unsigned char)newr)) {
                    if (cnt < 64) {
                        trans_patterns[state][cnt] = (unsigned char)newr;
                        ++cnt;
                    }
                }
            }
            trans_count[state] = cnt;
        }
    }
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!initialized) {
        init_transitions();
        initialized = true;
    }

    static seq_t dp1[STATES];
    static seq_t dp2[STATES];
    seq_t *cur = dp1;
    seq_t *next = dp2;

    /* initialise for the first row (prevrow = all zeros) */
    for (size_t i = 0; i < STATES; ++i) cur[i] = 0;
    for (unsigned int p = 0; p < (1U << WIDTH); ++p) {
        unsigned int state = p;     /* upr = 0, prv = p */
        cur[state] += 1;
    }

    /* add remaining rows */
    for (idx_t r = 1; r < n; ++r) {
        for (size_t i = 0; i < STATES; ++i) next[i] = 0;
        for (unsigned int state = 0; state < STATES; ++state) {
            seq_t val = cur[state];
            if (val == 0) continue;
            unsigned char prv   = state & ((1U << WIDTH) - 1);
            unsigned char *list = trans_patterns[state];
            unsigned char len   = trans_count[state];
            for (unsigned char k = 0; k < len; ++k) {
                unsigned char newr = list[k];
                unsigned int new_state = ((unsigned int)prv << WIDTH) | newr;
                next[new_state] += val;
            }
        }
        /* swap dp arrays */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* sum over final states where the bottom row satisfies the rules */
    seq_t total = 0;
    for (unsigned int state = 0; state < STATES; ++state) {
        seq_t val = cur[state];
        if (val == 0) continue;
        unsigned char upr = state >> WIDTH;
        unsigned char prv = state & ((1U << WIDTH) - 1);
        if (last_ok(upr, prv)) {
            total += val;
        }
    }
    return total;
}
