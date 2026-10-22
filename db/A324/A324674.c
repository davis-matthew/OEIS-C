/*
 * A324674  Starting at n, a(n) is the number of times we travel to a position
 *          already visited according to the following rules. On the k-th step
 *          (k=1,2,3,...) move a distance of k in the direction of zero. If
 *          the number landed on has been landed on before, move a distance of
 *          k away.
 * https://oeis.org/A324674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of times the walk starting at n and ending at 0
 *            (when the step size increases by 1 each time) lands on a
 *            position that has already been visited. The walk moves toward
 *            zero; if the intended landing position has been visited
 *            previously it moves away from zero instead. This is the defi...
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>


typedef struct {
    size_t cap;            /* capacity (always a power of two) */
    size_t sz;             /* number of stored keys */
    int64_t *key;          /* keys array */
    unsigned char *used;   /* 0 = empty, 1 = occupied */
} HashSet;

static void hs_init(HashSet *hs, size_t init_cap) {
    /* smallest power of two >= init_cap */
    size_t cap = 1;
    while (cap < init_cap) cap <<= 1;
    hs->cap = cap;
    hs->sz  = 0;
    hs->key = (int64_t *)malloc(cap * sizeof(int64_t));
    hs->used = (unsigned char *)calloc(cap, sizeof(unsigned char));
}

static void hs_free(HashSet *hs) {
    free(hs->key);
    free(hs->used);
}

static void hs_resize(HashSet *hs) {
    size_t new_cap = hs->cap << 1;
    int64_t *new_key = (int64_t *)malloc(new_cap * sizeof(int64_t));
    unsigned char *new_used = (unsigned char *)calloc(new_cap, sizeof(unsigned char));

    size_t mask = new_cap - 1;
    for (size_t i = 0; i < hs->cap; ++i) {
        if (hs->used[i]) {
            int64_t k = hs->key[i];
            size_t pos = ((uint64_t)k * 11400714819323198485ULL) & mask;
            while (new_used[pos]) pos = (pos + 1) & mask;
            new_used[pos] = 1;
            new_key[pos] = k;
        }
    }

    free(hs->key);
    free(hs->used);
    hs->key = new_key;
    hs->used = new_used;
    hs->cap = new_cap;
}

static int hs_contains(const HashSet *hs, int64_t k) {
    size_t mask = hs->cap - 1;
    size_t pos = ((uint64_t)k * 11400714819323198485ULL) & mask;
    while (hs->used[pos]) {
        if (hs->key[pos] == k) return 1;
        pos = (pos + 1) & mask;
    }
    return 0;
}

static void hs_add(HashSet *hs, int64_t k) {
    if (hs->sz * 5 >= hs->cap * 4)   /* load factor > 0.8 */
        hs_resize(hs);

    size_t mask = hs->cap - 1;
    size_t pos = ((uint64_t)k * 11400714819323198485ULL) & mask;
    while (hs->used[pos]) {
        if (hs->key[pos] == k) return;   /* already present */
        pos = (pos + 1) & mask;
    }
    hs->used[pos] = 1;
    hs->key[pos] = k;
    hs->sz++;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;      /* a(0) = 0 */

    int64_t cur = (int64_t)n;
    seq_t cnt = 0;

    HashSet hs;
    hs_init(&hs, 64);
    hs_add(&hs, cur);          /* starting point is visited */

    for (idx_t step = 1; cur != 0; ++step) {
        int64_t dir = (cur > 0) ? -1 : 1;     /* direction toward zero */
        int64_t desired = cur + dir * (int64_t)step;
        int64_t newcur;

        if (hs_contains(&hs, desired)) {
            /* target already visited – go away from zero */
            newcur = cur - dir * (int64_t)step;
        } else {
            newcur = desired;
        }

        /* count if the new position has already been visited before */
        if (hs_contains(&hs, newcur)) {
            cnt++;
        }

        hs_add(&hs, newcur);
        cur = newcur;          /* continue the walk */
    }

    hs_free(&hs);
    return cnt;
}
