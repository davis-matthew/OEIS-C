/*
 * A385817  Irregular triangle read by rows listing the lengths of maximal
 *          runs (sequences of consecutive elements increasing by 1) of binary
 *          indices, duplicate rows removed.
 * https://oeis.org/A385817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th element of the flattened list of run‑lengths
 * of the binary indices of natural numbers, outputting each unique composition
 * only once.  The first composition occurs at n = 1.  */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/* Structure to store a composition (array of run lengths). */
typedef struct {
    unsigned char *lens; /* run lengths, stored consecutively */
    size_t        len;   /* number of run lengths */
} comp_t;

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *flatseq = NULL;   /* flattened sequence of run lengths */
    static size_t flatcap = 0, flatlen = 0;
    static comp_t *seen = NULL;     /* array of seen compositions */
    static size_t seencap = 0, seencnt = 0;
    static idx_t  lastm = 0;        /* last m processed */

    /* if requested term already computed, return it */
    if (flatlen > (size_t)n)
        return flatseq[(size_t)n];

    /* compute terms until we reach the required index n */
    for (idx_t m = lastm + 1; flatlen <= (size_t)n; ++m) {
        /* ---------- compute run‑length composition for m ---------- */
        unsigned char runs[64];   /* enough for the small n considered */
        size_t rl_sz = 0;
        unsigned char curr_start = 0, last_pos = 0;
        bool in_run = false;

        idx_t x = m;
        unsigned char pos = 1;    /* position of the least significant bit */
        while (x != 0) {
            if (x & 1ULL) {
                if (!in_run) {                     /* start new run */
                    curr_start = pos;
                    in_run = true;
                } else if (pos == last_pos + 1) {   /* consecutive */
                    /* nothing special */
                } else {                           /* run interrupted */
                    runs[rl_sz++] = last_pos - curr_start + 1;
                    curr_start = pos;
                }
                last_pos = pos;
            }
            pos++;
            x >>= 1;
        }
        if (in_run) {   /* close the final run */
            runs[rl_sz++] = last_pos - curr_start + 1;
        }

        /* ---------- check if this composition has been seen ---------- */
        bool dup = false;
        for (size_t j = 0; j < seencnt; ++j) {
            if (seen[j].len == rl_sz &&
                memcmp(seen[j].lens, runs, rl_sz) == 0) {
                dup = true;
                break;
            }
        }

        /* ---------- store new composition and append its parts ---------- */
        if (!dup) {
            /* grow 'seen' array if necessary */
            if (seencnt == seencap) {
                size_t newcap = seencap ? seencap * 2 : 4;
                comp_t *tmp = (comp_t *)realloc(seen, newcap * sizeof(comp_t));
                seen = tmp;
                seencap = newcap;
            }

            /* copy run lengths */
            unsigned char *copy = (unsigned char *)malloc(rl_sz);
            memcpy(copy, runs, rl_sz);
            seen[seencnt].lens = copy;
            seen[seencnt].len  = rl_sz;
            seencnt++;

            /* append parts to the flattened sequence */
            for (size_t i = 0; i < rl_sz; ++i) {
                if (flatlen == flatcap) {
                    size_t newcap = flatcap ? flatcap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(flatseq, newcap * sizeof(seq_t));
                    flatseq = tmp;
                    flatcap = newcap;
                }
                flatseq[flatlen++] = (seq_t)runs[i];
            }
        }

        lastm = m;
    }

    return flatseq[(size_t)n];
}
