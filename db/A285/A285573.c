/*
 * A285573  Number of finite nonempty sets of pairwise indivisible divisors of
 *          n.
 * https://oeis.org/A285573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the nonempty antichains of the divisor poset of n.
 *            The function enumerates all antichains via backtracking over the
 *            set of divisors, using bitmasks to keep track of divisibility
 *            constraints
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

#define MAX_WORDS 4   /* covers up to 256 divisors (n <= 10000 => d < 200) */
typedef struct { uint64_t w[MAX_WORDS]; } mask_t;

static size_t g_words;            /* number of used words for current n   */
static size_t g_d;                /* number of divisors of current n      */
static mask_t *g_divMask;         /* divisibility masks for each divisor */

/* utility: check if a mask is empty */
static bool mask_is_zero(const mask_t *m)
{
    for (size_t i = 0; i < g_words; ++i)
        if (m->w[i]) return false;
    return true;
}

/* utility: get least set bit index (64bits) */
static int mask_first_bit(const mask_t *m)
{
    for (size_t i = 0; i < g_words; ++i) {
        uint64_t x = m->w[i];
        if (x) {
            return i * 64 + __builtin_ctzll(x);
        }
    }
    return -1; /* should not happen if mask not zero */
}

/* recursive DFS counting antichains (including empty) */
static seq_t dfs(mask_t avail)
{
    if (mask_is_zero(&avail)) return (seq_t)1;     /* only empty set */

    int idx = mask_first_bit(&avail);              /* smallest available element */
    int word_idx = idx >> 6;                       /* divide by 64 */
    int bit_pos  = idx & 63;

    /* exclude this element */
    mask_t excl = avail;
    excl.w[word_idx] &= ~(1ULL << bit_pos);
    seq_t count_excl = dfs(excl);

    /* include this element: remove all multiples of it */
    mask_t incl = avail;
    for (size_t w = 0; w < g_words; ++w)
        incl.w[w] &= ~g_divMask[idx].w[w];
    seq_t count_incl = dfs(incl);

    return count_excl + count_incl;
}

/* generate all divisors of n from its prime factorization */
static void gen_divisors(const uint64_t *pr, const uint64_t *ex,
                         size_t cnt, size_t pos, uint64_t cur,
                         uint64_t *buf, size_t *d)
{
    if (pos == cnt) {
        buf[*d] = cur;
        ++(*d);
        return;
    }
    uint64_t p = pr[pos];
    uint64_t val = 1;
    for (uint64_t e = 0; e <= ex[pos]; ++e) {
        gen_divisors(pr, ex, cnt, pos + 1, cur * val, buf, d);
        val *= p;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, safe return */

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* generate all divisors */
    uint64_t div_buf[256];
    size_t d = 0;
    gen_divisors(primes, exps, pcnt, 0, 1, div_buf, &d);
    g_d = d;
    g_words = (d + 63) >> 6;

    /* allocate and compute divisibility masks */
    g_divMask = (mask_t*)malloc(d * sizeof(mask_t));
    for (size_t i = 0; i < d; ++i) {
        for (size_t w = 0; w < g_words; ++w) g_divMask[i].w[w] = 0;
        for (size_t j = 0; j < d; ++j) {
            if (div_buf[j] % div_buf[i] == 0)
                g_divMask[i].w[j >> 6] |= (1ULL << (j & 63));
        }
    }

    /* initial mask with all divisors present */
    mask_t full;
    for (size_t w = 0; w < g_words; ++w) full.w[w] = 0;
    for (size_t i = 0; i < d; ++i)
        full.w[i >> 6] |= (1ULL << (i & 63));

    /* count antichains (include empty) */
    seq_t total = dfs(full);

    /* free mask array */
    free(g_divMask);

    /* remove the empty set */
    total = total - (seq_t)1;
    return total;
}
