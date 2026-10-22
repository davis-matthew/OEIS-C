/*
 * A353082  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest positive
 *          number that has not appeared that is a divisor of (a(n-1) +
 *          a(n-2))*|a(n-1) - a(n-2)|.
 * https://oeis.org/A353082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(1)=1, a(2)=2; for n>2, a(n) = the smallest positive integer
 *            not yet used that divides (a(n-1)+a(n-2))*|a(n-1)-a(n-2)|. If no
 *            such integer exists, the sequence terminates
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* Ensure a bool array can contain indices up to 'need' (inclusive). */
static inline void ensure_used_capacity(size_t need, bool **used, size_t *used_cap,
                                        size_t *max_used)
{
    if (need <= *max_used) return;
    size_t new_cap = *used_cap > 0 ? *used_cap : 1;
    while (new_cap <= need) new_cap <<= 1;
    bool *tmp = (bool *)realloc(*used, new_cap * sizeof(bool));
    if (!tmp) exit(1);          /* fatal if memory exhausted */
    /* Zero new entries. */
    for (size_t i = *used_cap; i < new_cap; ++i) tmp[i] = false;
    *used = tmp;
    *used_cap = new_cap;
    *max_used = need;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined: just return 0 for safety */

    /* Static data for memoisation and bookkeeping. */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;

    static bool *used = NULL;
    static size_t used_cap = 0;
    static size_t max_used = 0;

    static bool terminated = false;   /* whether the sequence has finished */

    /* --- Initialisation on first call ------------------------------------- */
    if (!seq) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(1);
        seq[0] = (seq_t)1;   /* a(1) */
        seq[1] = (seq_t)2;   /* a(2) */
        seq_len = 2;

        used_cap = 16;
        used = (bool *)calloc(used_cap, sizeof(bool));
        if (!used) exit(1);
        used[1] = true;
        used[2] = true;
        max_used = 2;
    }

    /* --- Build terms up to requested index n ------------------------------- */
    while (seq_len < n && !terminated) {
        seq_t a_prev2 = seq[seq_len - 2];
        seq_t a_prev1 = seq[seq_len - 1];

        seq_t sum = a_prev1 + a_prev2;
        seq_t diff = (a_prev1 > a_prev2) ? (a_prev1 - a_prev2) :
                                            (a_prev2 - a_prev1);
        seq_t prod = sum * diff;
        unsigned long long prod64 = (unsigned long long)prod;   /* safe cast */

        size_t best = 0;
        bool found = false;

        /* If product is zero, any positive integer divides it. */
        if (prod64 == 0) {
            size_t cand = 1;
            while (true) {
                ensure_used_capacity(cand, &used, &used_cap, &max_used);
                if (!used[cand]) break;
                ++cand;
            }
            best = cand;
            found = true;
        } else {
            /* Find smallest unused divisor of prod64. */
            unsigned long long r = (unsigned long long)sqrt((double)prod64);
            /* Correct possible rounding error */
            while ((r + 1) * (r + 1) <= prod64) ++r;
            while (r * r > prod64) --r;

            for (unsigned long long d = 1; d <= r; ++d) {
                if (prod64 % d != 0) continue;

                /* Check divisor d. */
                ensure_used_capacity((size_t)d, &used, &used_cap, &max_used);
                if (!used[(size_t)d]) {
                    if (!found || d < best) {
                        best = (size_t)d;
                        found = true;
                    }
                }

                /* Check complementary divisor prod/d. */
                unsigned long long d2 = prod64 / d;
                if (d2 != d) {
                    ensure_used_capacity((size_t)d2, &used, &used_cap, &max_used);
                    if (!used[(size_t)d2]) {
                        if (!found || d2 < best) {
                            best = (size_t)d2;
                            found = true;
                        }
                    }
                }
            }
        }

        if (!found) {
            terminated = true;
            break;   /* no new term possible */
        }

        /* Append the new term to the sequence. */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap << 1;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            seq = tmp;
            seq_cap = new_cap;
        }
        seq[seq_len] = (seq_t)best;
        seq_len++;

        /* Mark the chosen number as used. */
        ensure_used_capacity(best, &used, &used_cap, &max_used);
        used[best] = true;
    }

    /* Return the requested value if it exists; otherwise 0. */
    if (n <= seq_len) return seq[n - 1];
    return (seq_t)0;
}
