/*
 * A269343  Sequence that describes an infinite directed graph representing
 *          all of the combinations of a two-input logic gate from two
 *          starting inputs.
 * https://oeis.org/A269343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( a(n-1)*(a(n-1)-1)/2 ) * ( 1 + Sum_{k=0..n-1} a(k) ),
 *            a(0)=2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_values = NULL;
static seq_t *cumsum_values = NULL;
static idx_t max_n = 0;
static idx_t cap = 0;
static int initialized = 0;

static void init(void) {
    if (initialized) return;
    cap = 1;
    a_values = (seq_t *)malloc(cap * sizeof(seq_t));
    cumsum_values = (seq_t *)malloc(cap * sizeof(seq_t));
    a_values[0] = (seq_t)2;
    cumsum_values[0] = (seq_t)2;
    max_n = 0;
    initialized = 1;
}

static void ensure_capacity(idx_t n) {
    if (n < cap) return;
    idx_t new_cap = cap ? cap * 2 : 1;
    while (new_cap <= n) new_cap *= 2;
    a_values = (seq_t *)realloc(a_values, new_cap * sizeof(seq_t));
    cumsum_values = (seq_t *)realloc(cumsum_values, new_cap * sizeof(seq_t));
    cap = new_cap;
}

seq_t a(idx_t n) {
    init();
    if (n <= max_n) return a_values[(size_t)n];
    ensure_capacity(n);
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t prev = a_values[(size_t)(i - 1)];
        seq_t binom = (prev * (prev - (seq_t)1)) / (seq_t)2;
        seq_t factor = (seq_t)1 + cumsum_values[(size_t)(i - 1)];
        seq_t an = binom * factor;
        a_values[(size_t)i] = an;
        cumsum_values[(size_t)i] = cumsum_values[(size_t)(i - 1)] + an;
    }
    max_n = n;
    return a_values[(size_t)n];
}
