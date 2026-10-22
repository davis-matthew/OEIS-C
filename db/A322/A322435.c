/*
 * A322435  Number of pairs of factorizations of n into factors > 1 where no
 *          factor of the second divides any factor of the first.
 * https://oeis.org/A322435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs of factorizations of n into factors > 1
 *            with the property that no factor of the second divides any
 *            factor of the first
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* helper structure for one factorization, stored as a bitmask of divisors */
typedef struct {
    uint64_t *mask;      /* length = blocks */
} Fact;

/* recursive generation of all multiplicative partitions of n */
static void dfs(uint64_t rem, int start, uint64_t *divs, int ndiv, int blocks,
                uint64_t *cur, Fact **facts, size_t *cnt, size_t *cap) {
    if (rem == 1) {
        /* store a copy of current mask */
        if (*cnt == *cap) {
            size_t newcap = (*cap) ? (*cap) << 1 : 16;
            Fact *tmp = (Fact *)realloc(*facts, newcap * sizeof(Fact));
            if (!tmp) exit(1);
            *facts = tmp;
            *cap   = newcap;
        }
        Fact *f = &(*facts)[*cnt];
        f->mask = (uint64_t *)malloc(blocks * sizeof(uint64_t));
        if (!f->mask) exit(1);
        for (int b = 0; b < blocks; ++b) f->mask[b] = cur[b];
        ++(*cnt);
        return;
    }
    for (int i = start; i < ndiv; ++i) {
        uint64_t d = divs[i];
        if (rem % d == 0) {
            int b = i >> 6, off = i & 63;
            uint64_t old = cur[b];
            cur[b] |= ((uint64_t)1 << off);
            dfs(rem / d, i, divs, ndiv, blocks, cur, facts, cnt, cap);
            cur[b] = old;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;        /* a(1) = 1 by convention */

    /* ---- 1. list all divisors > 1 of n, sorted ---- */
    uint64_t divs[256];
    int ndiv = 0;
    for (uint64_t i = 2; i * i <= (uint64_t)n; ++i) {
        if ((uint64_t)n % i == 0) {
            divs[ndiv++] = i;
            uint64_t j = (uint64_t)n / i;
            if (j != i) divs[ndiv++] = j;
        }
    }
    divs[ndiv++] = (uint64_t)n;   /* include n itself */

    /* insertion sort (divs is tiny) */
    for (int i = 1; i < ndiv; ++i) {
        uint64_t key = divs[i];
        int j = i - 1;
        while (j >= 0 && divs[j] > key) {
            divs[j + 1] = divs[j];
            --j;
        }
        divs[j + 1] = key;
    }

    int blocks = (ndiv + 63) >> 6;    /* number of 64-bit words per mask */

    /* ---- 2. pre‑compute for each divisor the set of divisors it divides ---- */
    uint64_t *multiple = (uint64_t *)malloc(ndiv * blocks * sizeof(uint64_t));
    if (!multiple) exit(1);
    for (int i = 0; i < ndiv; ++i)
        for (int b = 0; b < blocks; ++b)
            multiple[(size_t)i * blocks + b] = 0;

    for (int i = 0; i < ndiv; ++i) {
        for (int j = 0; j < ndiv; ++j) {
            if (divs[j] % divs[i] == 0) {
                int b = j >> 6, off = j & 63;
                multiple[(size_t)i * blocks + b] |= ((uint64_t)1 << off);
            }
        }
    }

    /* ---- 3. enumerate all factorizations ---- */
    Fact *facts = NULL;
    size_t cnt = 0, cap = 0;
    uint64_t *cur = (uint64_t *)calloc(blocks, sizeof(uint64_t));
    if (!cur) exit(1);

    dfs((uint64_t)n, 0, divs, ndiv, blocks, cur, &facts, &cnt, &cap);
    free(cur);

    /* ---- 4. count admissible ordered pairs ---- */
    seq_t ans = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t *mask_i = facts[i].mask;
        for (size_t j = 0; j < cnt; ++j) {
            uint64_t *mask_j = facts[j].mask;
            bool ok = true;
            for (int b = 0; b < blocks && ok; ++b) {
                uint64_t bits = mask_j[b];
                while (bits && ok) {
                    int tz   = __builtin_ctzll(bits);
                    int idx2 = (b << 6) + tz;   /* divisor index in mask_j */
                    uint64_t *m_base = multiple + (size_t)idx2 * blocks;
                    for (int k = 0; k < blocks; ++k) {
                        if (mask_i[k] & m_base[k]) {
                            ok = false;
                            break;
                        }
                    }
                    bits &= bits - 1;   /* clear lowest set bit */
                }
            }
            if (ok) ++ans;
        }
    }

    /* ---- 5. clean up ---- */
    for (size_t k = 0; k < cnt; ++k) free(facts[k].mask);
    free(facts);
    free(multiple);

    return ans;
}
