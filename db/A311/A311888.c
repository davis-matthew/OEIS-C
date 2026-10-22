/*
 * A311888  Coordination sequence Gal.4.45.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=8, a(3)=13, a(4)=17, a(5)=21, a(6)=27,
 *            a(7)=31, a(8)=35, a(9)=39, a(10)=44, a(11)=49, a(12)=50,
 *            a(13)=57, a(14)=63, a(15)=65, a(16)=71, a(17)=73, a(18)=80; For
 *            n>18:
 *            a(n)=2*a(n‑1)‑3*a(n‑2)+4*a(n‑3)‑5*a(n‑4)+6*a(n‑5)‑7*a(n‑6)+8*a(n‑7)
 *            ‑7*a(n‑8)+6*a(n‑9)‑5*a(n‑10)+4*a(n‑11)‑3*a...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[19] = {
    1, 4, 8, 13, 17, 21, 27, 31, 35, 39,
    44, 49, 50, 57, 63, 65, 71, 73, 80
};

static seq_t *cache = NULL;
static size_t cap = 0;                /* allocated capacity */
static size_t last_filled = 18;        /* index of last known value */

static void ensure_capacity(size_t need)
{
    if (!cache) {
        cap = need > 32 ? need * 2 : 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        for (size_t i = 0; i < 19; ++i) cache[i] = init[i];
    } else if (need > cap) {
        size_t newcap = cap * 2;
        if (newcap < need) newcap = need;
        seq_t *p = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!p) exit(1);
        cache = p;
        cap = newcap;
    }
}

seq_t a(idx_t n)
{
    if (n < 19) return init[(size_t)n];

    ensure_capacity((size_t)n + 1);

    /* Compute missing terms in ascending order */
    for (size_t i = last_filled + 1; i <= (size_t)n; ++i) {
        seq_t val = 0;
        val += 2 * cache[i-1];
        val -= 3 * cache[i-2];
        val += 4 * cache[i-3];
        val -= 5 * cache[i-4];
        val += 6 * cache[i-5];
        val -= 7 * cache[i-6];
        val += 8 * cache[i-7];
        val -= 7 * cache[i-8];
        val += 6 * cache[i-9];
        val -= 5 * cache[i-10];
        val += 4 * cache[i-11];
        val -= 3 * cache[i-12];
        val += 2 * cache[i-13];
        val -= 1 * cache[i-14];
        cache[i] = val;
    }
    last_filled = (size_t)n;
    return cache[(size_t)n];
}
