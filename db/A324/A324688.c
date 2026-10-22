/*
 * A324688  Starting at n, a(n) is the difference of the number of times we
 *          move away from zero from positive spots and the number of times we
 *          move away from zero from negative spots according to the following
 *          rules. On the k-th step (k=1,2,3,...) move a distance of k in the
 *          direction of zero. If the number...
 * https://oeis.org/A324688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is defined as follows – start at position n, then for k =
 *            1,2,3,… move a distance k towards 0; if that position has
 *            already been visited, instead move the same distance away from
 *            0. Count the number of times you move away from 0 when starting
 *            from a positive position minus the number of time...
 * Range:     i8 n <= 16, i16 n <= 16, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--------- dynamic hash set for int64_t keys ---------*/
typedef struct {
    int64_t *keys;        /* key values   */
    bool    *occ;        /* occupancy flag */
    size_t   size;       /* table size – power of two */
    size_t   count;      /* number of stored keys   */
    size_t   mask;       /* size-1, for modulo */
} VisitedSet;

/* initial size (must be a power of two) */
#define VISIT_INIT_SIZE (1 << 10)

/* multiplicative hash for int64_t */
static inline size_t hash_int64(int64_t k, size_t mask)
{
    return ((size_t)k * 11995408973635179863ULL) & mask;
}

/* create a new set */
static VisitedSet *visited_new(void)
{
    VisitedSet *s = (VisitedSet *)malloc(sizeof(VisitedSet));
    s->size = VISIT_INIT_SIZE;
    s->mask = s->size - 1;
    s->count = 0;
    s->keys  = (int64_t *)malloc(s->size * sizeof(int64_t));
    s->occ   = (bool *)calloc(s->size, sizeof(bool)); /* all false */
    return s;
}

/* free a set */
static void visited_free(VisitedSet *s)
{
    free(s->keys);
    free(s->occ);
    free(s);
}

/* rehash to double the size */
static void visited_rehash(VisitedSet *s)
{
    size_t old_size = s->size;
    int64_t *old_keys = s->keys;
    bool   *old_occ  = s->occ;

    s->size <<= 1;
    s->mask  = s->size - 1;
    s->keys  = (int64_t *)malloc(s->size * sizeof(int64_t));
    s->occ   = (bool *)calloc(s->size, sizeof(bool));
    s->count = 0;

    for (size_t i = 0; i < old_size; ++i) {
        if (old_occ[i]) {
            /* re-insert key */
            int64_t key = old_keys[i];
            size_t idx = hash_int64(key, s->mask);
            while (s->occ[idx]) idx = (idx + 1) & s->mask;
            s->keys[idx] = key;
            s->occ[idx]  = true;
            ++s->count;
        }
    }
    free(old_keys);
    free(old_occ);
}

/* insert key, resizing if necessary */
static void visited_insert(VisitedSet *s, int64_t k)
{
    if (s->count * 2 >= s->size) visited_rehash(s);

    size_t idx = hash_int64(k, s->mask);
    while (s->occ[idx]) {
        if (s->keys[idx] == k) return;   /* already present */
        idx = (idx + 1) & s->mask;
    }
    s->keys[idx] = k;
    s->occ[idx]  = true;
    ++s->count;
}

/* test membership */
static bool visited_contains(VisitedSet *s, int64_t k)
{
    size_t idx = hash_int64(k, s->mask);
    while (s->occ[idx]) {
        if (s->keys[idx] == k) return true;
        idx = (idx + 1) & s->mask;
    }
    return false;
}

/*--------------------- the requested function ---------------------*/
seq_t a(idx_t n)
{
    /* Special case: starting at 0 reaches 0 immediately */
    if (n == 0) return (seq_t)0;

    VisitedSet *mv = visited_new();          /* visited positions */
    int64_t cur = (int64_t)n;                 /* current position */
    visited_insert(mv, cur);

    seq_t diff = 0;                          /* result accumulator */
    idx_t step = 1;

    while (cur != 0) {
        int64_t dir = (cur > 0) ? -1 : 1;          /* towards zero */
        int64_t target = cur + dir * (int64_t)step;

        if (visited_contains(mv, target)) {
            /* move away from zero */
            cur = cur - dir * (int64_t)step;  /* opposite direction */
            /* update diff according to the sign before moving */
            if (cur > 0) diff += (seq_t)1;
            else if (cur < 0) diff -= (seq_t)1;
            visited_insert(mv, cur);
        } else {
            /* move towards zero */
            cur = target;
            visited_insert(mv, cur);
        }
        ++step;
    }

    visited_free(mv);
    return diff;
}
