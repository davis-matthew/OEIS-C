/*
 * A256789  R(k), the minimal alternating squares representation of k,
 *          concatenated for k = 0, 1, 2,....
 * https://oeis.org/A256789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabf
 * Formula:   a(n) is the concatenation of all signed terms in the minimal *
 *            alternating squares representations R(k) for k = 0,1,2,…. * The
 *            representation R(k) satisfies * * R(0) = 0 * R(1) = 1 * R(2) = 4
 *            – 2 * For k > 2 * let B(k) = smallest perfect square ≥ k (B(k) =
 *            ceil(sqrt(k))²) * then * R(k) = B(k) – R...
 * Range:     i8 n <= 150, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Helper: compute the smallest perfect square ≥ n */
static seq_t smallest_square(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t s = floor_sqrt(x);           /* floor sqrt */
    if (s * s < x) ++s;                /* ceil sqrt */
    return s * s;
}

/* Cache for lengths of representations */
static idx_t *len_cache = NULL;
static size_t len_cap   = 0;

/* Ensure that len_cache has room for index n */
static void ensure_len_cache(idx_t n)
{
    if (n < len_cap) return;
    size_t new_cap = (len_cap > 0) ? 2 * len_cap : 1;
    while (new_cap <= (size_t)n) new_cap *= 2;
    len_cache = (idx_t *)realloc(len_cache, new_cap * sizeof(idx_t));
    for (size_t i = len_cap; i < new_cap; ++i) len_cache[i] = 0;
    len_cap = new_cap;
    /* initialise base values once */
    if (!len_cache[0]) len_cache[0] = 1;
    if (!len_cache[1]) len_cache[1] = 1;
    /* R(2) has length 2; we set it lazily in lenRec */
}

/* Recursive length of R(n) */
static idx_t rep_len(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;

    ensure_len_cache(n);
    if (len_cache[n] != 0) return len_cache[n];

    seq_t Bval = smallest_square(n);
    idx_t m   = (idx_t)(Bval - (seq_t)n);

    if (m == 0) {
        len_cache[n] = 1;
    } else {
        len_cache[n] = 1 + rep_len(m);
    }
    return len_cache[n];
}

/* Recursive term of R(n) at position idx (0‑based) */
static seq_t rep_term(idx_t n, idx_t idx)
{
    if (n == 0) {
        /* representation is [0] */
        return (idx == 0) ? 0 : 0;      /* idx>0 never occurs */
    }
    if (n == 1) {
        return (idx == 0) ? 1 : 0;
    }
    if (n == 2) {
        return (idx == 0) ? (seq_t)4 : (seq_t)-2;
    }

    seq_t Bval = smallest_square(n);
    idx_t m   = (idx_t)(Bval - (seq_t)n);

    if (idx == 0) return Bval;

    /* m > 0 here – valid recursive step */
    seq_t t = rep_term(m, idx - 1);
    return -t;
}

/* Public function requested by the build */
seq_t a(idx_t n)
{
    idx_t cumulative = 0;
    idx_t k = 0;
    while (1) {
        idx_t lenk = rep_len(k);
        if (cumulative + lenk > n) break;
        cumulative += lenk;
        ++k;
    }
    idx_t offset = n - cumulative;
    return rep_term(k, offset);
}
