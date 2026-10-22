/*
 * A324687  Starting at n, a(n) is the number of times we move away from zero
 *          from a positive position according to the following rules. On the
 *          k-th step (k=1,2,3,...) move a distance of k in the direction of
 *          zero. If the number landed on has been landed on before, move a
 *          distance of k away.
 * https://oeis.org/A324687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A324687
 * a(n) = number of times that the walk described in the OEIS entry moves
 * away from zero while currently on a positive position.
 * The process: start at position n. For k = 1,2,3,... move a distance k
 * towards zero; if the target has been visited before, move a distance k
 * away from zero instead.  Record a step as “away from zero” only when
 * the new position is positive.  The walk terminates when position 0 is
 * reached.  The function below simulates this walk using a hash set of
 * visited positions.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Simple open-addressing hash set for 64‑bit integers.
 * 0 is used as the empty slot marker; the walk never needs to store 0
 * because the process stops immediately when reaching position 0. */
#define SET_INIT_CAP 64
#define SET_LOAD_FACTOR 0.5

typedef struct {
    int64_t *table;
    size_t cap;   /* power of two */
    size_t num;
} Int64Set;

static inline uint64_t hash_int64(int64_t x)
{
    /* 64‑bit multiplicative hashing (splitmix64 style) */
    uint64_t z = (uint64_t)x + 0x9e3779b97f4a7c15ULL;
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

static Int64Set *set_create(void)
{
    Int64Set *s = (Int64Set *)malloc(sizeof(*s));
    if (!s) return NULL;
    s->cap = SET_INIT_CAP;
    s->num = 0;
    s->table = (int64_t *)calloc(s->cap, sizeof(int64_t));
    if (!s->table) { free(s); return NULL; }
    return s;
}

static void set_destroy(Int64Set *s)
{
    free(s->table);
    free(s);
}

static void set_ensure_capacity(Int64Set *s)
{
    if ((double)s->num / s->cap > SET_LOAD_FACTOR) {
        size_t newcap = s->cap * 2;
        int64_t *newtab = (int64_t *)calloc(newcap, sizeof(int64_t));
        if (!newtab) return; /* out of memory, will be handled by caller */
        for (size_t i = 0; i < s->cap; ++i) {
            int64_t v = s->table[i];
            if (v) {
                size_t idx = hash_int64(v) & (newcap - 1);
                while (newtab[idx])
                    idx = (idx + 1) & (newcap - 1);
                newtab[idx] = v;
            }
        }
        free(s->table);
        s->table = newtab;
        s->cap = newcap;
    }
}

static bool set_contains(const Int64Set *s, int64_t key)
{
    size_t idx = hash_int64(key) & (s->cap - 1);
    while (s->table[idx]) {
        if (s->table[idx] == key)
            return true;
        idx = (idx + 1) & (s->cap - 1);
    }
    return false;
}

static void set_insert(Int64Set *s, int64_t key)
{
    if (!key) return; /* 0 is never inserted */
    set_ensure_capacity(s);
    size_t idx = hash_int64(key) & (s->cap - 1);
    while (s->table[idx]) {
        if (s->table[idx] == key)
            return; /* already present */
        idx = (idx + 1) & (s->cap - 1);
    }
    s->table[idx] = key;
    ++s->num;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    int64_t cur = (int64_t)n;
    Int64Set *visited = set_create();
    if (!visited)
        return (seq_t)0; /* on allocation failure return 0 */

    set_insert(visited, cur);

    seq_t cnt = 0;
    for (idx_t k = 1; ; ++k) {
        if (cur == 0)
            break;  /* just in case */

        int64_t target;
        if (cur > 0)
            target = cur - (int64_t)k;
        else
            target = cur + (int64_t)k;

        if (target == 0) {
            cur = 0;
            break;  /* reached zero while moving toward it */
        }

        if (!set_contains(visited, target)) {
            /* move toward zero */
            cur = target;
            set_insert(visited, cur);
        } else {
            /* move away from zero */
            int64_t away = (cur > 0) ? cur + (int64_t)k : cur - (int64_t)k;
            cur = away;
            if (away > 0)
                ++cnt;
            set_insert(visited, cur);
            if (cur == 0)
                break;
        }
    }

    set_destroy(visited);
    return cnt;
}
