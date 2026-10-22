/*
 * A355476  a(1)=1. For a(n) a novel term, a(n+1) = A000005(a(n)). For a(n)
 *          seen already k > 1 times, a(n+1) = k*a(n).
 * https://oeis.org/A355476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1. For n>1 let m = a(n-1). Count c = number of times m has
 *            appeared in a(1)..a(n-1) (i.e. including this one). If c==1
 *            (novel term) then a(n)=d(m) (divisor count of m). If c>1 then
 *            a(n)=c*m. (All arithmetic is on unsigned integer type seq_t.)
 * Range:     u8 n <= 67, u16 n <= 700, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ↑ this implements the OEIS recurrence for A355476. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"  /* provides divisor_count_u64(uint64_t) */

#define MAX_TERMS 20000   /* array capacity; >10 000, safe for requested limits */

static seq_t  seq[MAX_TERMS];
static size_t seq_len  = 0;   /* number of terms currently stored (≥1 after init) */

struct map_entry {
    seq_t val;
    unsigned count;          /* ≥1 */
};

static struct map_entry map[MAX_TERMS];
static size_t map_len  = 0;   /* number of distinct values seen so far */

/* linear search of value in map; returns index or MAP_NPOS */
#define MAP_NPOS ((size_t)-1)

static size_t find_map_index(seq_t v)
{
    for (size_t i = 0; i < map_len; ++i) {
        if (map[i].val == v) return i;
    }
    return MAP_NPOS;
}

/* compute divisor count for seq_t values that fit in uint64_t */
static inline seq_t divisor_count(seq_t x)
{
    return (seq_t)divisor_count_u64((uint64_t)x);
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, return 0 */
        return (seq_t)0;

    /* initialise on first use */
    if (seq_len == 0) {
        seq[0] = (seq_t)1;
        seq_len = 1;
        map[0].val   = (seq_t)1;
        map[0].count = 1;
        map_len = 1;
    }

    /* produce terms up to requested index */
    /* assuming n <= MAX_TERMS; otherwise we silently truncate */
    if (n > MAX_TERMS) n = MAX_TERMS;

    while (seq_len < n) {
        seq_t cur = seq[seq_len - 1];
        size_t cur_idx = find_map_index(cur);   /* guaranteed to exist */
        unsigned cur_count = map[cur_idx].count;  /* ≥1 */

        seq_t next;
        if (cur_count == 1) {                   /* novel term */
            next = divisor_count(cur);
        } else {                                /* repeated term */
            next = (seq_t)cur_count * cur;
        }

        /* append next */
        seq[seq_len] = next;
        ++seq_len;

        /* update map for 'next' */
        size_t nxt_idx = find_map_index(next);
        if (nxt_idx == MAP_NPOS) {
            /* new value */
            map[map_len].val   = next;
            map[map_len].count = 1;
            ++map_len;
        } else {
            /* existing value: increment count */
            ++map[nxt_idx].count;
        }
    }

    return seq[n - 1];
}
