/*
 * A335374  Numbers k such that the k-th composition in standard order
 *          (A066099) is not co-unimodal.
 * https://oeis.org/A335374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th term of OEIS sequence A335374: the list of numbers k such that the k-th composition in standard order (graded reverse‑lexicographic, A066099) is not co‑unimodal. */

#include <stddef.h>
#include <stdbool.h>

#define CACHE_CAP 200000   /* adequate for n up to several ten thousand */

static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;
static idx_t last_k = 0;

/* Return the n-th term (n >= 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */

    if (cache_len >= (size_t)n) return cache[n-1];

    seq_t comp[65];
    size_t len;

    for (idx_t k = last_k + 1;; ++k) {        /* iterate k until enough terms are found */
        /* compute composition corresponding to k */
        len = 0;
        idx_t tmp = k;
        size_t pos = 1;
        size_t prev = 0;
        while (tmp > 0) {
            if (tmp & 1) {
                comp[len++] = pos - prev;
                prev = pos;
            }
            ++pos;
            tmp >>= 1;
        }

        /* test co‑unimodal property */
        bool inc_psum[65];
        bool dec_suf[65];

        inc_psum[0] = true;
        for (size_t i = 1; i < len; ++i)
            inc_psum[i] = inc_psum[i-1] && (comp[i] <= comp[i-1]);

        dec_suf[len-1] = true;
        if (len >= 2) {
            for (size_t i = len-2;; --i) {
                dec_suf[i] = dec_suf[i+1] && (comp[i] <= comp[i+1]);
                if (i == 0) break;
            }
        }

        bool is_co = false;
        for (size_t s = 0; s <= len; ++s) {
            bool pref = (s == 0) || inc_psum[s-1];
            bool suff = (s == len) || dec_suf[s];
            if (pref && suff) { is_co = true; break; }
        }

        if (!is_co) {                         /* not co‑unimodal → store k */
            if (cache_len >= CACHE_CAP) return 0; /* safety guard */
            cache[cache_len++] = (seq_t)k;
        }

        if (cache_len >= (size_t)n) {         /* enough terms collected */
            last_k = k;                       /* remember last processed k */
            break;
        }
    }

    return cache[n-1];
}
