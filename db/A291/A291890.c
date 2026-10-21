/*
 * A291890  Number of symmetrically unique Dyck paths of semilength n and
 *          height six.
 * https://oeis.org/A291890
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 21, u64 n <= 39, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291890: Number of symmetrically unique Dyck paths of semilength n and height six
   (offset 6).  The sequence satisfies the linear recurrence

        a(n) = 12·a(n‑1) – 51·a(n‑2) + 66·a(n‑3) + 144·a(n‑4) – 540·a(n‑5)
               + 411·a(n‑6) + 510·a(n‑7) – 1006·a(n‑8) + 380·a(n‑9)
               + 282·a(n‑10) – 284·a(n‑11) + 84·a(n‑12) – 8·a(n‑13),

   for all n ≥ 19.  The first 13 values (a(6) … a(18)) are given below:
          a(6)=1, a(7)=6, a(8)=41, a(9)=209, a(10)=1005, a(11)=4411,
          a(12)=18546, a(13)=74854, a(14)=294293, a(15)=1131635,
          a(16)=4280686, a(17)=15974526, a(18)=58974616. */

#include <stdlib.h>

static const int RECUR[13] = {
    12, -51, 66, 144, -540, 411, 510, -1006, 380,
    282, -284, 84, -8
};

static const seq_t BASE[13] = {
    (seq_t)1,    (seq_t)6,    (seq_t)41,    (seq_t)209,
    (seq_t)1005, (seq_t)4411, (seq_t)18546, (seq_t)74854,
    (seq_t)294293, (seq_t)1131635, (seq_t)4280686,
    (seq_t)15974526, (seq_t)58974616
};

seq_t a(idx_t n) {
    if (n < 6) return (seq_t)0;          /* indices below the offset are irrelevant */

    const idx_t off = n - 6;              /* index into the stored array */

    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* capacity of cache in elements */
    static idx_t last_computed = 6;       /* highest n that has been computed */

    /* first time: allocate base array */
    if (cache == NULL) {
        cache_cap = 13;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failure (unlikely) */
        for (int i = 0; i < 13; ++i) cache[i] = BASE[i];
        last_computed = 18;                     /* a(18) is the last pre‑filled term */
    }

    /* grow capacity if necessary */
    if ((size_t)off + 1 > cache_cap) {
        size_t new_cap = (size_t)off + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;             /* allocation failure (unlikely) */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms after the pre‑filled block */
    if (n > last_computed) {
        for (idx_t cur = last_computed + 1; cur <= n; ++cur) {
            size_t idx = (size_t)(cur - 6);     /* index of a(cur) in cache */
            seq_t sum = (seq_t)0;
            for (int j = 1; j <= 13; ++j) {
                int coeff = RECUR[j - 1];
                seq_t prev = cache[idx - j];
                if (coeff >= 0)
                    sum += (seq_t)coeff * prev;
                else
                    sum -= (seq_t)(-coeff) * prev;
            }
            cache[idx] = sum;
        }
        last_computed = n;
    }

    return cache[off];
}
