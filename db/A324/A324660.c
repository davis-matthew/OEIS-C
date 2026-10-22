/*
 * A324660  Starting at n, a(n) is the total number of moves made to the right
 *          according to the following rules. On the k-th step (k=1,2,3,...)
 *          move a distance of k in the direction of zero. If the number
 *          landed on has been landed on before, move a distance of k away
 *          from zero instead.
 * https://oeis.org/A324660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of moves made to the right during the walk
 *            starting at position n according to A324660's rules. The walk
 *            stops when 0 is first visited. The returned value is strictly
 *            computed in seq_t for any n >= 0
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ---------- Simple open-address hash table for 64‑bit integers ----------
   Only non‑zero keys are stored.  0 is used as the empty marker.       */

static size_t hash_int64(int64_t x)
{
    uint64_t u = (uint64_t)x;
    u ^= u >> 33;
    u *= 0xff51afd7ed558ccdULL;
    u ^= u >> 33;
    u *= 0xc4ceb9fe1a85ec53ULL;
    u ^= u >> 33;
    return (size_t)u;
}

/* Resize the table to a new power‑of‑two capacity. */
static void ht_reserve(int64_t **tab, size_t *cap, size_t new_cap)
{
    int64_t *new_tbl = (int64_t *)calloc(new_cap, sizeof(int64_t));
    for (size_t i = 0; i < *cap; ++i) {
        int64_t k = (*tab)[i];
        if (k != 0) {
            size_t h = hash_int64(k) & (new_cap - 1);
            while (new_tbl[h] != 0)
                h = (h + 1) & (new_cap - 1);
            new_tbl[h] = k;
        }
    }
    free(*tab);
    *tab  = new_tbl;
    *cap  = new_cap;
}

/* Test membership.  Key 0 is never inserted. */
static int ht_contains(int64_t *tab, size_t cap, int64_t key)
{
    if (key == 0)
        return 0;
    size_t h = hash_int64(key) & (cap - 1);
    while (1) {
        int64_t k = tab[h];
        if (k == 0)
            return 0;
        if (k == key)
            return 1;
        h = (h + 1) & (cap - 1);
    }
}

/* Insert key; return 1 if inserted anew, 0 if already present.  */
static int ht_insert(int64_t *tab, size_t cap, int64_t key, size_t *used)
{
    if (key == 0)
        return 0;
    size_t h = hash_int64(key) & (cap - 1);
    while (1) {
        int64_t k = tab[h];
        if (k == 0) {
            tab[h] = key;
            ++(*used);
            return 1;
        }
        if (k == key)   /* already present */
            return 0;
        h = (h + 1) & (cap - 1);
    }
}

/* ---------- The required sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Initial table: power of two size, empty entries are 0. */
    size_t cap   = 8;
    size_t used  = 0;
    int64_t *tab = (int64_t *)calloc(cap, sizeof(int64_t));

    seq_t right = 0;
    int64_t pos = (int64_t)n;

    /* Insert the starting position. */
    ht_insert(tab, cap, pos, &used);

    idx_t step = 1;
    while (1) {
        /* Proposed target towards zero. */
        int64_t target = (pos > 0) ? pos - (int64_t)step
                                   : pos + (int64_t)step;
        int visited_target = ht_contains(tab, cap, target);

        int64_t newpos;
        if (!visited_target) {
            newpos = target;
            if (newpos > pos)
                ++right;
        } else {
            newpos = (pos > 0) ? pos + (int64_t)step
                               : pos - (int64_t)step;
            if (newpos > pos)
                ++right;
        }

        /* Grow table if load factor exceeds 0.5. */
        if (used * 2 >= cap)
            ht_reserve(&tab, &cap, cap * 2);

        ht_insert(tab, cap, newpos, &used);

        if (newpos == 0)
            break;

        pos = newpos;
        ++step;
    }

    free(tab);
    return right;
}
