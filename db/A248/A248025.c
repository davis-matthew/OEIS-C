/*
 * A248025  Lexicographically earliest permutation of the positive integers
 *          such that the first digit of a(n+1) is the digital root of a(n).
 * https://oeis.org/A248025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the lexicographically earliest permutation of the
 *            positive integers such that the first digit of a(n+1) equals the
 *            digital root of a(n). The implementation generates terms on
 *            demand by greedily picking the smallest unused integer whose
 *            leading digit matches the required digital root
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _digit_len, _digital_root, _next_with_digit, _pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_digit_len.h"
#include "../utils/_digital_root.h"
#include "../utils/_next_with_digit.h"
#include "../utils/_pow10.h"

/* Forward declarations of static helpers */
static inline int _digit_len(seq_t);
static inline seq_t _pow10(int);
static seq_t _next_with_digit(int d, seq_t cur);
static inline int _digital_root(seq_t v);

/* Static state: cache of generated terms and smallest unused candidate per first digit */
static seq_t *g_seq = NULL;     /* array of generated terms */
static size_t   g_len = 0;      /* number of terms stored */
static size_t   g_cap = 0;      /* current allocated capacity for g_seq */
static seq_t   g_cand[10] = {0};/* g_cand[d] = smallest unused number starting with digit d (1‑9) */

/* Ensure that g_seq has capacity for at least 'need' elements */
static void _ensure_capacity(size_t need)
{
    if (need <= g_cap) return;
    size_t new_cap = g_cap ? g_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(g_seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    g_seq = tmp;
    g_cap = new_cap;
}

/* Initialise static state for the very first call */
static void _init(void)
{
    _ensure_capacity(128);
    /* First term is 1 */
    g_seq[0] = 1;
    g_len = 1;
    /* Initialise candidates: for digit 1, next after 1 is 10; others start at themselves */
    g_cand[1] = _next_with_digit(1, 1);   /* 10 */
    for (int d = 2; d <= 9; ++d) g_cand[d] = d;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1; n==0 is unused but defined safely */

    _ensure_capacity((size_t)n);

    /* Initialise on first use */
    if (g_len == 0) _init();

    /* Generate terms until we reach index n */
    while (g_len < (size_t)n) {
        int d = _digital_root(g_seq[g_len - 1]);          /* required leading digit */
        seq_t val = g_cand[d];                            /* smallest unused with that digit */
        g_seq[g_len] = val;                               /* store new term */
        g_len++;                                          /* advance length */
        g_cand[d] = _next_with_digit(d, val);             /* update candidate for digit d */
    }
    return g_seq[n - 1];
}
