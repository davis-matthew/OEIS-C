/*
 * A389607  A bisection of A221075.
 * https://oeis.org/A389607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Let s_0 = 1, s_1 = 5, and for k ≥ 2 s_k = 4 * s_{k-1} – s_{k-2}.
 *            Then for n ≥ 1 if n is even (n = 2k) a(n) = s_k^2 – 1, if n is
 *            odd (n = 2k–1) a(n) = 2 * s_k * s_{k-1} + 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *s_table = NULL;
static size_t s_capacity = 0;   // allocated size
static idx_t  s_last    = 0;    // largest k for which s_k is known

/* Ensure that s_k is available for all k ≤ need_k. */
static void ensure_s(idx_t need_k)
{
    if (need_k <= s_last) return;

    /* Resize if necessary. */
    size_t new_cap = (size_t)(need_k + 1);
    if (!s_table) {
        s_table = (seq_t*)malloc(sizeof(seq_t) * new_cap);
        if (!s_table) exit(1);          /* out of memory */
        s_capacity = new_cap;
    } else if (new_cap > s_capacity) {
        seq_t *tmp = (seq_t*)realloc(s_table, sizeof(seq_t) * new_cap);
        if (!tmp) exit(1);
        s_table   = tmp;
        s_capacity = new_cap;
    }

    /* Initialise the base values. */
    if (s_last == 0) s_table[0] = (seq_t)1;
    if (need_k >= 1 && s_last < 1) s_table[1] = (seq_t)5;

    /* Compute missing s-values using the recurrence. */
    idx_t start = (s_last >= 2) ? s_last + 1 : 2;
    for (idx_t i = start; i <= need_k; ++i) {
        seq_t temp = (seq_t)4 * s_table[i-1];
        temp -= s_table[i-2];
        s_table[i] = temp;
    }
    s_last = need_k;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    /* Map n to the appropriate k. */
    idx_t k = (n & 1) ? ((n + 1) >> 1) : (n >> 1);  /* n odd → (n+1)/2, else n/2 */

    /* Compute s_k and s_{k-1} on demand. */
    ensure_s(k);

    seq_t sk   = s_table[k];
    seq_t result;

    if ((n & 1) == 0) {                  /* even index: a(2k) */
        result = sk * sk;
        result -= (seq_t)1;
    } else {                              /* odd index: a(2k-1) */
        seq_t skm1 = s_table[k-1];
        result = sk * skm1;
        result += result;                 /* multiply by 2 */
        result += (seq_t)2;
    }
    return result;
}
