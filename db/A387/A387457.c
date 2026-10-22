/*
 * A387457  Number of Hamiltonian paths in the n-Goldberg graph.
 * https://oeis.org/A387457
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 6*a(n-1) - 8*a(n-2) - 16*a(n-3) + 44*a(n-4) * -
 *            8*a(n-5) - 48*a(n-6) + 32*a(n-7) for n ≥ 13 * Initial values
 *            (a(3) … a(12)) are stored explicitly below
 * Range:     u8 none, u16 n <= 5, u32 n <= 17, u64 n <= 46, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_values[13] = {
    0, 0, 0,                                      /* 0…2 are unused */
    (seq_t)2772,     /* a(3)  */
    (seq_t)13096,    /* a(4)  */
    (seq_t)43400,    /* a(5)  */
    (seq_t)142680,   /* a(6)  */
    (seq_t)413168,   /* a(7)  */
    (seq_t)1162304,  /* a(8)  */
    (seq_t)3080736,  /* a(9)  */
    (seq_t)7966880,  /* a(10) */
    (seq_t)19932352, /* a(11) */
    (seq_t)48943488  /* a(12) */
};

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;  /* array of computed terms */
    static idx_t  capacity   = 0;     /* current allocated size */
    static idx_t  last_done  = 0;     /* largest index already filled */

    /* Values are defined only for n ≥ 3 */
    if (n < 3) return 0;

    /* Expand the cache if necessary */
    if (capacity <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* out of memory – return 0 as fallback */
        cache    = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n (if any) */
    if (last_done < n) {
        idx_t start = last_done < 3 ? 3 : last_done + 1;
        for (idx_t i = start; i <= n; ++i) {
            if (i <= 12) {
                cache[i] = init_values[i];
            } else {
                seq_t v = cache[i-1] * (seq_t)6;
                v += cache[i-4] * (seq_t)44;
                v += cache[i-7] * (seq_t)32;

                seq_t sub = cache[i-2] * (seq_t)8;
                sub += cache[i-3] * (seq_t)16;
                sub += cache[i-5] * (seq_t)8;
                sub += cache[i-6] * (seq_t)48;

                v -= sub;
                cache[i] = v;
            }
        }
        last_done = n;
    }

    return cache[n];
}
