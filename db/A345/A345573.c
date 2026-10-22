/*
 * A345573  Numbers that are the sum of seven fourth powers in seven or more
 *          ways.
 * https://oeis.org/A345573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 759, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A345573: Numbers that are the sum of seven fourth powers
   in seven or more unordered ways.
   The function enumerates all non‑decreasing 7‑tuples of positive
   integers, counts how many times each sum occurs, and stores the
   sums that appear at least seven times.  The list is built once
   on first use and reused thereafter. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* -------------------------------------------------------------------- */
/*  Internal state                                                     */
/* -------------------------------------------------------------------- */
static seq_t *terms = NULL;      /* array of sequence values            */
static size_t term_count = 0;    /* number of stored terms              */
static bool   initialized = false; /* has init_terms finished?          */

/* -------------------------------------------------------------------- */
/*  Forward declaration                                               */
/* -------------------------------------------------------------------- */
static void init_terms(void);

/* -------------------------------------------------------------------- */
/*  Sequence entry function                                            */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized)
        init_terms();

    if (n < 1 || n > (idx_t)term_count)
        return (seq_t)0;          /* out of range */

    return terms[n - 1];           /* a(1) is first element */
}

/* -------------------------------------------------------------------- */
/*  Initialization routine                                             */
/* -------------------------------------------------------------------- */
static void init_terms(void)
{
    /* ---------------------------------------------------------------- *
     *  Set an upper bound on the sums to be investigated.  20 million
     *  is sufficient for the first 20 000 terms (the sequence grows
     *  far slower than the sum of 7 fourth powers).                      *
     * ---------------------------------------------------------------- */
    const uint32_t LIMIT = 20000000u;   /* 2×10⁷ */

    /* ---------------------------------------------------------------- *
     *  Find the largest index i such that i⁴ ≤ LIMIT.        *
     * ---------------------------------------------------------------- */
    int maxIndex = 0;
    while (1) {
        uint64_t t = (uint64_t)(maxIndex + 1);
        t *= t;     t *= t;            /* (maxIndex + 1)⁴ */
        if (t > LIMIT)
            break;
        ++maxIndex;
    }

    /* ---------------------------------------------------------------- *
     *  Pre‑compute i⁴ for 1 ≤ i ≤ maxIndex.                           *
     * ---------------------------------------------------------------- */
    int *pow4 = (int *)malloc((maxIndex + 1) * sizeof(*pow4));
    if (!pow4)
        return;                 /* allocation failed – give up */

    pow4[0] = 0;  /* unused, keeps indexing simple */
    for (int i = 1; i <= maxIndex; ++i) {
        uint64_t t = (uint64_t)i * i * i * i;
        pow4[i] = (int)t;        /* fits in 32 bits for i ≤ 66 */
    }

    /* ---------------------------------------------------------------- *
     *  Count representations of each possible sum.                     *
     * ---------------------------------------------------------------- */
    uint16_t *cnt = (uint16_t *)calloc((size_t)LIMIT + 1, sizeof(*cnt));
    if (!cnt) {
        free(pow4);
        return;
    }

    for (int a = 1; a <= maxIndex; ++a) {
        uint32_t pa = (uint32_t)pow4[a];
        if ((uint64_t)pa * 7ULL > LIMIT) break;

        for (int b = a; b <= maxIndex; ++b) {
            uint32_t sum_ab = pa + (uint32_t)pow4[b];
            if ((uint64_t)pow4[b] * 6ULL + sum_ab > LIMIT) break;

            for (int c = b; c <= maxIndex; ++c) {
                uint32_t sum_abc = sum_ab + (uint32_t)pow4[c];
                if ((uint64_t)pow4[c] * 5ULL + sum_abc > LIMIT) break;

                for (int d = c; d <= maxIndex; ++d) {
                    uint32_t sum_abcd = sum_abc + (uint32_t)pow4[d];
                    if ((uint64_t)pow4[d] * 4ULL + sum_abcd > LIMIT) break;

                    for (int e = d; e <= maxIndex; ++e) {
                        uint32_t sum_abcde = sum_abcd + (uint32_t)pow4[e];
                        if ((uint64_t)pow4[e] * 3ULL + sum_abcde > LIMIT) break;

                        for (int f = e; f <= maxIndex; ++f) {
                            uint32_t sum_abcdef = sum_abcde + (uint32_t)pow4[f];
                            for (int g = f; g <= maxIndex; ++g) {
                                uint32_t sum = sum_abcdef + (uint32_t)pow4[g];
                                if (sum > LIMIT)
                                    break;
                                ++cnt[sum];
                            }
                        }
                    }
                }
            }
        }
    }

    /* ---------------------------------------------------------------- *
     *  Collect sums that appear at least seven times.                  *
     * ---------------------------------------------------------------- */
    size_t capacity = 20000;          /* initial capacity */
    terms = (seq_t *)malloc(capacity * sizeof(*terms));
    if (!terms) {
        free(pow4);
        free(cnt);
        return;
    }

    for (uint32_t s = 0; s <= LIMIT; ++s) {
        if (cnt[s] >= 7) {
            if (term_count >= capacity) {
                capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(terms, capacity * sizeof(*terms));
                if (!tmp) {
                    free(pow4);
                    free(cnt);
                    return;
                }
                terms = tmp;
            }
            terms[term_count++] = (seq_t)s;
        }
    }

    /* ---------------------------------------------------------------- *
     *  Clean up temporary structures.                                  *
     * ---------------------------------------------------------------- */
    free(pow4);
    free(cnt);
    initialized = true;
}
