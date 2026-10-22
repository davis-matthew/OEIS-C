/*
 * A324683  Starting at n, a(n) is the number of times we move from a negative
 *          position to a spot we have already visited according to the
 *          following rules. On the k-th step (k=1,2,3,...) move a distance of
 *          k in the direction of zero. If the number landed on has been
 *          landed on before, move a distance of k away.
 * https://oeis.org/A324683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   hs_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: simulate the walk described in the OEIS definition.  Starting
   at the operand n, we perform successive steps k = 1,2,3,...:
   1.  Compute the intended move toward zero:
          if cur > 0  → cur - k
          if cur < 0  → cur + k
   2.  If that intended position has been visited before, move away
          (cur + k if cur > 0, otherwise cur - k).
      Otherwise, move to the intended position.
   3.  If the starting position cur is negative and the new position
      has been visited before, increment the counter.
   4.  Mark the new position as visited and continue until cur == 0.
   The counter is returned as seq_t.  An auxiliary hash table stores
   visited positions as signed 64‑bit integers.  The table expands
   when the load factor exceeds ½.  All arithmetic is in signed
   64‑bit integers; the counter uses seq_t. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/hs_index.h"

/* ------------------------------------------------------------------ */
/*   Hash table for signed 64‑bit keys passed by value (no collisions
 *   are allowed if used correctly).  The table size is always a power
 *   of two, allowing a simple modulo function with bit masking.     */
struct hs_slot {
    int64_t key;
    unsigned char used;
};

#define HS_INIT_CAP (1 << 16)          /* 65536 buckets – more than enough */

static bool
hs_has(const struct hs_slot *tab, size_t cap, int64_t key)
{
    size_t i = hs_index(tab, cap, key);
    while (tab[i].used) {
        if (tab[i].key == key)
            return true;
        i = (i + 1) & (cap - 1);
    }
    return false;
}

static void
hs_put(struct hs_slot **table_ret, size_t *cap_ret, size_t *used_ret,
       int64_t key)
{
    struct hs_slot *tab = *table_ret;
    size_t cap = *cap_ret;
    size_t used = *used_ret;

    /* Rehash when load factor exceeds ½; this keeps probing times small. */
    if (used + 1 > (cap >> 1)) {
        size_t newcap = cap << 1;
        struct hs_slot *newtab = (struct hs_slot *)calloc(newcap,
                                                          sizeof(struct hs_slot));
        if (!newtab)               /* allocation failure – abort silently. */
            return;
        for (size_t j = 0; j < cap; ++j) {
            if (tab[j].used) {
                int64_t k = tab[j].key;
                size_t i = hs_index(newtab, newcap, k);
                while (newtab[i].used)
                    i = (i + 1) & (newcap - 1);
                newtab[i].key = k;
                newtab[i].used = 1;
            }
        }
        free(tab);
        tab = newtab;
        cap = newcap;
    }

    size_t i = hs_index(tab, cap, key);
    while (tab[i].used) {
        if (tab[i].key == key)
            return;  /* already present */
        i = (i + 1) & (cap - 1);
    }
    tab[i].key = key;
    tab[i].used = 1;

    *table_ret = tab;
    *cap_ret   = cap;
    *used_ret  = used + 1;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    int64_t cur = (int64_t)n;   /* current position – may become negative   */
    seq_t   count = 0;          /* answer                                 */

    size_t cap  = HS_INIT_CAP;
    size_t used = 0;
    struct hs_slot *table = (struct hs_slot *)calloc(cap, sizeof(struct hs_slot));
    if (!table)
        return count;          /* allocation failure → return 0             */

    hs_put(&table, &cap, &used, cur);   /* starting position visited */

    int64_t step = 1;
    while (cur != 0) {
        int64_t intended;
        if (cur > 0)
            intended = cur - step;     /* move toward zero   */
        else
            intended = cur + step;     /* move toward zero   */

        int64_t nxt;
        if (hs_has(table, cap, intended))
            nxt = cur + (cur > 0 ? step : -step); /* move away           */
        else
            nxt = intended;                          /* normal move        */

        if (cur < 0 && hs_has(table, cap, nxt))
            count++;

        hs_put(&table, &cap, &used, nxt);
        cur   = nxt;
        step += 1;
    }

    free(table);
    return count;
}
