/*
 * A282840  Lexicographic-first permutation of the nonnegative integers such
 *          that the partial sums (-1)^issquare(a(n))*a(n) always remain
 *          nonnegative.
 * https://oeis.org/A282840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 3570, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the lexicographic‑first permutation of
 * the non‑negative integers such that the partial sums
 * S(n) = S(n‑1) + a(n)   if a(n) is a nonsquare
 * S(n) = S(n‑1) – a(n)   if a(n) is a square
 * never become negative.  At each step the smallest unused
 * integer that keeps the running sum non‑negative is chosen.
 *
 * The implementation keeps a working set of used values,
 * the current signed sum S, the next unused square root,
 * and the next candidate nonsquare.  The sequence is
 * memoised so that calls to a() with arbitrary indices
 * are answered in O(n) time for the largest argument.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* integer square‑root for unsigned 64‑bit values.
   Returns floor(sqrt(x)).  It uses binary search with
   __int128 arithmetic to avoid overflow. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;   /* (hi)^2 > 2^64-1 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * (unsigned __int128)mid <= (unsigned __int128)x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Returns true if value is a perfect square. */
static bool is_square(idx_t v)
{
    idx_t r = isqrt_u64(v);
    return (unsigned __int128)r * r == (unsigned __int128)v;
}

/* ---------------------------------------------------- */

seq_t a(idx_t n)
{
    /* persistent state --------------------------------------------------- */
    static seq_t *seq = NULL;          /* sequence values */
    static idx_t seq_cap = 0;          /* capacity of seq array */
    static idx_t seq_len = 0;          /* next free index in seq */

    static char *used = NULL;          /* flag array: used[value] == 1 */
    static idx_t used_cap = 0;         /* capacity of used array */

    static seq_t S = 0;                /* running signed sum */
    static idx_t next_sq_root = 1;     /* next unused square root (root^2) */
    static idx_t next_nonsq = 1;       /* smallest unused nonsquare */

    /* first initialisation ---------------------------------------------- */
    if (!seq) {
        seq_cap = 1;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)0;   /* a(0) = 0 */
        seq_len = 1;

        used_cap = 1024;
        used = (char *)calloc(used_cap, sizeof(char));
        used[0] = 1;         /* 0 is used */

        S = 0;
    }

    /* generate terms up to requested index -------------------------------- */
    while (seq_len <= n) {
        seq_t value;          /* a(seq_len) */

        /* compute current smallest unused square */
        idx_t sq_root = next_sq_root;
        unsigned __int128 sq_val128 = (unsigned __int128)sq_root * sq_root;
        unsigned __int128 sum128 = (unsigned __int128)S;

        if (sum128 >= sq_val128) { /* can subtract the smallest unused square */
            value = (seq_t)sq_val128;
            idx_t idx = (idx_t)value;

            /* enlarge used array if necessary */
            if (idx >= used_cap) {
                idx_t new_cap = used_cap ? used_cap : 1;
                while (idx >= new_cap) new_cap <<= 1;
                char *tmp = (char *)realloc(used, new_cap * sizeof(char));
                memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(char));
                used = tmp;
                used_cap = new_cap;
            }
            used[idx] = 1;
            S -= value;
            next_sq_root++;            /* move to next square root */
        } else {                       /* must add a nonsquare */
            idx_t cand = next_nonsq;
            while (1) {
                /* expand used array if needed */
                if (cand >= used_cap) {
                    idx_t new_cap = used_cap ? used_cap : 1;
                    while (cand >= new_cap) new_cap <<= 1;
                    char *tmp = (char *)realloc(used, new_cap * sizeof(char));
                    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(char));
                    used = tmp;
                    used_cap = new_cap;
                }
                if (used[cand]) {          /* already used */
                    cand++;
                    continue;
                }
                if (is_square(cand)) {     /* skip squares */
                    cand++;
                    continue;
                }
                break;                     /* found smallest unused nonsquare */
            }
            value = (seq_t)cand;
            used[cand] = 1;
            S += value;
            next_nonsq = cand + 1;
        }

        /* append the new value to seq ------------------------------------- */
        if (seq_len >= seq_cap) {
            idx_t new_cap = seq_cap ? seq_cap : 1;
            while (seq_len >= new_cap) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq = tmp;
            seq_cap = new_cap;
        }
        seq[seq_len] = value;
        seq_len++;
    }

    return seq[n];
}
