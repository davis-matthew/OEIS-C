/*
 * A349491  a(1)=1, a(2)=4; for n > 2, a(n) is the smallest unused positive
 *          number such that gcd(a(n-1)*n,a(n)) > 1, where a(n) != a(n-1) and
 *          a(n) != n.
 * https://oeis.org/A349491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n):  a(1)=1, a(2)=4; for n>2, a(n) is the smallest unused
   positive integer, not equal to a(n‑1) or n, such that
   gcd(a(n-1)*n , a(n)) > 1.                     */
/* Implementation uses a memoised sequential construction, suitable for
   any order of calls.                                       */

#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

static seq_t *seq = NULL;       /* computed terms */
static size_t seq_cap = 0;      /* allocated capacity for seq */
static idx_t seq_len = 0;       /* number of terms stored */

static uint8_t *used = NULL;    /* bitmap of used positive integers */
static size_t used_cap = 0;     /* allocated capacity for used */

/* Ensure seq has space for at least n terms. */
static void ensure_seq_cap(idx_t n) {
    if (seq_cap < (size_t)n) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Ensure used can index up to val (inclusive).  New slots are zeroed. */
static void ensure_used_cap(idx_t val) {
    if ((size_t)val >= used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 8;
        while (new_cap <= (size_t)val) new_cap *= 2;
        used = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
        memset(used + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
        used_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for callers outside range */

    ensure_seq_cap(n);

    /* Return cached value if already computed. */
    if (seq_len >= n) return seq[n-1];

    /* Compute missing terms up to n. */
    for (idx_t cur = seq_len + 1; cur <= n; ++cur) {
        seq_t value = 0;

        if (cur == 1) {
            value = (seq_t)1;
        } else if (cur == 2) {
            value = (seq_t)4;
        } else {
            /* Search for the smallest unused candidate. */
            const seq_t prev = seq[cur-2];
            seq_t product = prev * (seq_t)cur;

            idx_t candidate = 1;
            for (;;) {
                /* Skip forbidden values. */
                if ((seq_t)candidate == prev || (seq_t)candidate == (seq_t)cur) {
                    ++candidate;
                    continue;
                }

                /* Make sure we can index used. */
                ensure_used_cap(candidate);

                if (used[(size_t)candidate]) {
                    ++candidate;
                    continue;
                }

                /* Check gcd condition. */
                seq_t g = gcd(product, (seq_t)candidate);
                if (g != (seq_t)1) {
                    value = (seq_t)candidate;   /* Found the term. */
                    break;
                }

                ++candidate;
            }
        }

        seq[cur-1] = value;

        /* Mark the value as used. */
        ensure_used_cap((idx_t)value);
        used[(size_t)value] = 1;
    }

    seq_len = n;
    return seq[n-1];
}
