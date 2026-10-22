/*
 * A379165  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest unused
 *          positive number that is either coprime to both a(n-1) and a(n-2)
 *          or shares a factor with both a(n-1) and a(n-2).
 * https://oeis.org/A379165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; for n>2, a(n) is the smallest unused positive
 *            integer k such that (gcd(k,a(n-1))==1 && gcd(k,a(n-2))==1) OR
 *            (gcd(k,a(n-1))>1 && gcd(k,a(n-2))>1)
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* forward declarations of helpers */
static void grow_seq(size_t min_cap);
static void grow_used(idx_t min_cap);

/* static state */
static bool initialized = false;
static idx_t max_n = 0;
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static bool *used = NULL;
static size_t used_cap = 0;

/* ensure seq has space for indices up to min_cap-1 */
static void grow_seq(size_t min_cap) {
    if (min_cap <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 1;
    while (new_cap < min_cap)
        new_cap *= 2;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_seq)
        abort();
    if (new_seq != seq)
        seq = new_seq;
    for (size_t i = seq_cap; i < new_cap; ++i)
        seq[i] = (seq_t)0;
    seq_cap = new_cap;
}

/* ensure used has space for indices up to min_cap-1 */
static void grow_used(idx_t min_cap) {
    if ((size_t)min_cap <= used_cap)
        return;
    size_t new_cap = used_cap ? used_cap : 1;
    while (new_cap < (size_t)min_cap)
        new_cap *= 2;
    bool *new_used = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!new_used)
        abort();
    if (new_used != used)
        used = new_used;
    for (size_t i = used_cap; i < new_cap; ++i)
        used[i] = false;
    used_cap = new_cap;
}

seq_t a(idx_t n) {
    /* lazy initialization */
    if (!initialized) {
        initialized = true;
        /* allocate minimal capacity for first two terms */
        grow_seq(3);
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)2;
        max_n = 2;
        grow_used(3);           /* indices 0,1,2 */
        used[1] = true;
        used[2] = true;
    }

    if (n <= max_n)
        return seq[n];

    /* extend storage for new terms */
    grow_seq(n + 1);

    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t a1 = seq[i - 1];
        seq_t a0 = seq[i - 2];

        /* find smallest unused k satisfying the rule */
        idx_t cand;
        for (cand = 1; ; ++cand) {
            /* ensure used array covers candidate */
            grow_used((idx_t)cand + 1);
            if (used[cand])
                continue;
            seq_t cand_seq = (seq_t)cand;
            seq_t g1 = gcd(a1, cand_seq);
            seq_t g0 = gcd(a0, cand_seq);
            bool coprime = (g1 == (seq_t)1 && g0 == (seq_t)1);
            bool share   = (g1 > (seq_t)1 && g0 > (seq_t)1);
            if (coprime || share)
                break;
        }

        seq[i] = (seq_t)cand;
        used[cand] = true;
    }

    max_n = n;
    return seq[n];
}
