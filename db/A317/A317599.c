/*
 * A317599  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 5, 6, 7 or 8 king-move adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A317599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-3) - 34*a(n-4) - 13*a(n-5) \ + 8*a(n-6)
 *            + 114*a(n-7) + 36*a(n-8) - 48*a(n-9) - 68*a(n-10) \ + 13*a(n-11)
 *            + 14*a(n-12) - 21*a(n-13) - 21*a(n-14) - 22*a(n-15) - 8*a(n-16)
 *            Initial values: a(1)=4, a(2)=23, a(3)=100, a(4)=435, a(5)=1959,
 *            a(6)=8872, a(7)=40080, a(8)=181116, a(9)...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const seq_t init[18] = {
    0ULL, /* unused index 0 */
    4ULL, 23ULL, 100ULL, 435ULL, 1959ULL, 8872ULL, 40080ULL,
    181116ULL, 818827ULL, 3702099ULL, 16737863ULL, 75676531ULL,
    342158969ULL, 1547018148ULL, 6994611112ULL, 31625128427ULL,
    142988537981ULL
};

typedef struct { idx_t k; seq_t coeff; } term_t;

static const term_t pos_terms[] = {
    { 1, 5ULL }, { 6, 8ULL }, { 7, 114ULL }, { 8, 36ULL },
    { 11, 13ULL }, { 12, 14ULL }
};
static const size_t pos_n = sizeof(pos_terms)/sizeof(pos_terms[0]);

static const term_t neg_terms[] = {
    { 3, 2ULL }, { 4, 34ULL }, { 5, 13ULL },
    { 9, 48ULL }, { 10, 68ULL },
    { 13, 21ULL }, { 14, 21ULL }, { 15, 22ULL }, { 16, 8ULL }
};
static const size_t neg_n = sizeof(neg_terms)/sizeof(neg_terms[0]);

/* Ensure the cache has room for elements 0..needed-1   */
static void ensure_capacity(idx_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if ((idx_t)(*cap) >= needed) return;
    size_t new_cap = (*cap) ? (*cap)*2 : 16;
    if (new_cap < (size_t)needed) new_cap = needed;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but safe */
    if (n <= 17) return init[n];

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 0; /* highest index computed */

    /* first time: initialise cache with the fixed initial values */
    if (cache == NULL) {
        ensure_capacity(18, &cap, (void**)&cache, sizeof(seq_t));
        for (idx_t i = 0; i < 18; ++i) cache[i] = init[i];
        last = 17;
    }

    /* if needed compute up to n */
    if (n > last) {
        ensure_capacity(n + 1, &cap, (void**)&cache, sizeof(seq_t));
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;
            for (size_t t = 0; t < pos_n; ++t) {
                pos_sum += pos_terms[t].coeff * cache[i - pos_terms[t].k];
            }
            for (size_t t = 0; t < neg_n; ++t) {
                neg_sum += neg_terms[t].coeff * cache[i - neg_terms[t].k];
            }
            cache[i] = pos_sum - neg_sum; /* positive sum > negative sum */
        }
        last = n;
    }

    return cache[n];
}
