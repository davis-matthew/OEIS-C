/*
 * A296752  Numbers whose base-13 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          < #(falls); see Comments.
 * https://oeis.org/A296752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A296752: numbers whose base‑13 digits have #(rises) < #(falls). */
#include <stdint.h>
#include <stddef.h>

#define MAXCACHE 20000           /* enough for n up to ~10 000 */

static seq_t cache[MAXCACHE];   /* stored terms */
static idx_t cache_len = 0;     /* how many terms are stored */
static seq_t next_candidate = (seq_t)1;   /* next number to test */

seq_t a(idx_t n)
{
    /* generate terms lazily until we have at least n of them */
    while (cache_len < n && cache_len < MAXCACHE) {
        seq_t cand = next_candidate;  /* current number to examine */

        /* extract base‑13 digits (least significant first) */
        uint8_t digs[32];
        size_t len = 0;
        seq_t tmp = cand;
        while (tmp > (seq_t)0) {
            seq_t q = tmp / (seq_t)13;
            uint8_t r = (uint8_t)(tmp - q * (seq_t)13); /* tmp % 13 */
            digs[len++] = r;
            tmp = q;
        }
        if (len == 0) {          /* should not happen for cand >= 1 */
            len = 1;
            digs[0] = 0;
        }

        /* count rises and falls in the order d(0)…d(len‑1)
           where d(0) is the most significant digit */
        int rises = 0;
        int falls = 0;
        for (size_t i = 0; i + 1 < len; ++i) {
            seq_t hi = (seq_t)digs[len - 1 - i];     /* most significant of the pair */
            seq_t lo = (seq_t)digs[len - 2 - i];
            if (hi < lo)        rises++;
            else if (hi > lo)   falls++;
        }

        if (rises < falls) {        /* qualifies for the sequence */
            cache[cache_len++] = cand;
        }

        next_candidate = cand + (seq_t)1;   /* advance to next integer */
    }

    /* n is guaranteed to be ≤ cache_len due to MAXCACHE */
    return cache[n - 1];
}
