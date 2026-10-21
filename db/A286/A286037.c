/*
 * A286037  a(n) = T(A285984(n))^2, where T(m) is the m-th triangular number
 *          A000217(m).
 * https://oeis.org/A286037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Helpers:   floor_sqrt, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (T(b(n)))^2 ,
 * where b(n) = A285984(n) is given by
 *   b(0)=0,   b(1)=110,
 *   b(n) = 264·sqrt(27·T(b(n‑2))+1)+b(n‑4)   (n≥2),
 * and T(m)=m(m+1)/2 is the m‑th triangular number.
 */
#include <stddef.h>                     /* size_t */
#include "../utils/floor_sqrt.h"        /* floor_sqrt(seq_t) */
#include "../utils/seq_realloc.h"       /* seq_realloc(seq_t*,size_t,size_t) */

/* Helper: triangular number computed without overflow for 64‑bit seq_t */
static inline seq_t tri(seq_t m) {
    /* compute m*(m+1)/2 by dividing first */
    if ((m & 1) == 0)
        return (m / 2) * (m + 1);
    else
        return m * ((m + 1) / 2);
}

/* Cache for the b(n) values */
static seq_t *b_cache = NULL;   /* array of b(n) values  */
static size_t b_cap   = 0;      /* allocated capacity           */
static size_t b_len   = 0;      /* number of values computed    */

/* grow the cache to accommodate at least 'cap' elements */
static void ensure_capacity(size_t cap)
{
    if (cap <= b_cap) return;
    size_t new_cap = (b_cap == 0) ? 16 : b_cap * 2;
    if (new_cap < cap) new_cap = cap;
    b_cache = seq_realloc(b_cache, b_cap, new_cap);
    b_cap   = new_cap;
}

/* compute b(n) for all n ≤ given index, inclusive */
static void ensure_b_up_to(idx_t n)
{
    if ((size_t)n < b_len) return;   /* already computed */

    ensure_capacity((size_t)n + 1);

    for (idx_t idx = (idx_t)b_len; idx <= n; ++idx) {
        seq_t val;
        if (idx == 0) {
            val = (seq_t)0;
        } else if (idx == 1) {
            val = (seq_t)110;
        } else {
            /* previous values needed */
            seq_t b_n2 = b_cache[idx - 2];   /* b(n-2)  */
            seq_t b_n4;
            if (idx >= 4)
                b_n4 = b_cache[idx - 4];     /* b(n-4)  */
            else if (idx == 2)
                b_n4 = (seq_t)110;           /* b(-2)   */
            else
                b_n4 = (seq_t)0;             /* b(-1)   */

            /* compute 27·T(b(n‑2))+1 and its square root */
            seq_t tr   = b_n2 * (b_n2 + 1) / 2;     /* T(b(n‑2)) */
            seq_t tmp  = (seq_t)27 * tr + 1;
            seq_t sq   = floor_sqrt(tmp);

            val = (seq_t)264 * sq + b_n4;
        }
        b_cache[b_len++] = val;
    }
}

/* public function: a(n) = (T(b(n)))^2 */
seq_t a(idx_t n)
{
    ensure_b_up_to(n);

    seq_t bn = b_cache[(size_t)n];
    seq_t t  = tri(bn);          /* T(b(n)) */
    return t * t;                /* square of the triangular number */
}
