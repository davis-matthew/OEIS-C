/*
 * A324689  List of all starting positions for which no position is revisited
 *          according to the following rules. On the k-th step (k=1,2,3,...)
 *          move a distance of k in the direction of zero. If the number
 *          landed on has been landed on before, move a distance of k away.
 * https://oeis.org/A324689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: Enumerates starting positions that never revisit a point
 * following the rule: on step k, move a distance k towards zero;
 * if the new position has been visited, move a distance k away from
 * zero instead.  The function returns the nth such starting position
 * (a(0) = 0).  It searches candidates in increasing order and simulates
 * the walk; the first time a previously visited position is reached the
 * candidate is discarded, otherwise when the walk reaches 0 it is
 * accepted.  Implementation is O(n) in practice for n up to ~10^4. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- restrictions and helper definitions ------------------------------- */

/* (seq_t and idx_t are defined by the build; see problem statement) */
#define SEQ_T_DECL seq_t
#define IDX_T_DECL idx_t

/* type for signed integers large enough to hold any visited position */
#if defined(__SIZEOF_INT128__)
typedef __int128 signed_big_int;
#else
typedef long long signed_big_int;
#endif

/* simple open‐addressing hash set for signed_big_int keys (C only) */
typedef struct {
    signed_big_int *keys;     /* keys array  */
    unsigned char *occupied; /* 0=empty,1=full */
    size_t capacity;          /* power of two */
    size_t size;              /* number of elements */
} VisitedSet;

/* hash function for signed_big_int using lower 32 bits */
static inline size_t sb_hash(signed_big_int k)
{
    return (size_t)((unsigned long long)k & 0xffffffffULL);
}

/* initialize a visited set with given power‑of‑two capacity */
static void visited_init(VisitedSet *s, size_t capacity)
{
    s->keys = (signed_big_int*)malloc(capacity * sizeof(signed_big_int));
    s->occupied = (unsigned char*)calloc(capacity, 1);
    s->capacity = capacity;
    s->size = 0;
}

/* free a visited set */
static void visited_free(VisitedSet *s)
{
    free(s->keys);
    free(s->occupied);
}

/* rehash into a new larger table (double size) */
static void visited_rehash(VisitedSet *s)
{
    size_t new_cap = s->capacity * 2;
    signed_big_int *new_keys = (signed_big_int*)malloc(new_cap * sizeof(signed_big_int));
    unsigned char *new_occ = (unsigned char*)calloc(new_cap, 1);
    for (size_t i = 0; i < s->capacity; ++i) {
        if (s->occupied[i]) {
            signed_big_int key = s->keys[i];
            size_t h = sb_hash(key) & (new_cap - 1);
            while (new_occ[h]) h = (h + 1) & (new_cap - 1);
            new_keys[h] = key;
            new_occ[h] = 1;
        }
    }
    free(s->keys);
    free(s->occupied);
    s->keys = new_keys;
    s->occupied = new_occ;
    s->capacity = new_cap;
}

/* return true if key is already present; otherwise insert it */
static bool visited_insert_check(VisitedSet *s, signed_big_int key)
{
    size_t h = sb_hash(key) & (s->capacity - 1);
    while (s->occupied[h]) {
        if (s->keys[h] == key) return true;  /* found */
        h = (h + 1) & (s->capacity - 1);
    }
    /* insert */
    s->keys[h] = key;
    s->occupied[h] = 1;
    ++s->size;
    /* rehash if load factor > 0.5 */
    if (s->size > s->capacity / 2) visited_rehash(s);
    return false;
}

/* --- simulation of the walk -------------------------------------------- */

/* return true if starting position `start` never revisits a point */
static bool is_safe(seq_t start)
{
    VisitedSet set;
    visited_init(&set, 16);          /* small initial table */
    visited_insert_check(&set, (signed_big_int)start);
    signed_big_int pos = (signed_big_int)start;

    for (idx_t k = 1; ; ++k) {
        signed_big_int nxt;
        if (pos > 0)          nxt = pos - (signed_big_int)k;
        else if (pos < 0)     nxt = pos + (signed_big_int)k;
        else                  return true;   /* reached zero safely */

        /* if projected position already visited, move away instead */
        if (visited_insert_check(&set, nxt)) {
            if (pos > 0)      nxt = pos + (signed_big_int)k;
            else              nxt = pos - (signed_big_int)k;

            /* if away position still visited -> unsafe */
            if (visited_insert_check(&set, nxt)) {
                visited_free(&set);
                return false;
            }
        }

        /* now nxt is a new position */
        pos = nxt;
    }

    /* unreachable, but keep compiler happy */
    visited_free(&set);
    return false;
}

/* --- memoised sequence computation ------------------------------------- */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_start = 0;

    /* grow cache array when needed */
    if (cache_len == 0) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* compute until we have enough terms */
    while (cache_len <= n) {
        seq_t cand = (seq_t)next_start;
        if (is_safe(cand)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cand;
        }
        ++next_start;
    }

    return cache[n];
}
