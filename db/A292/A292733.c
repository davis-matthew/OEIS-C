/*
 * A292733  Numbers in which 3 outnumbers all other digits together.
 * https://oeis.org/A292733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 769, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 994 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searching for numbers in which digit 3 outnumbers all other digits together */

#include <stdlib.h>
#include <stdbool.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static bool is_more_three(seq_t x)
{
    unsigned int cnt3 = 0;
    unsigned int total = 0;
    const seq_t ten = (seq_t)10;
    while (x > (seq_t)0) {
        unsigned int d = (unsigned int)(x % ten);
        if (d == 3u) cnt3++;
        total++;
        x = x / ten;
    }
    return cnt3 > total - cnt3;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* undefined for offset 1 */

    /* static cache of found terms */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static idx_t terms_len = 0;
    static seq_t next_val = (seq_t)0;

    /* expand cache until we have at least n terms */
    while (terms_len < n) {
        next_val++; /* test the next integer */
        if (is_more_three(next_val)) {
            /* enlarge cache if necessary */
            if ((size_t)terms_len == terms_cap) {
                size_t new_cap = terms_cap == 0 ? 16 : terms_cap << 1;
                seq_t *tmp = seq_realloc(terms, terms_cap, new_cap);
                if (!tmp) exit(1); /* out of memory */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = next_val;
        }
    }
    return terms[n - 1];
}
