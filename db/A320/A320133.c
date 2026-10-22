/*
 * A320133  Number of integer solutions to a^2 + b^2 + 2*c^2 + 11*d^2 = n.
 * https://oeis.org/A320133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS sequence A320133: Number of integer solutions to
 *     a^2 + b^2 + 2*c^2 + 11*d^2 = n
 * with a,b,c,d ∈ ℤ.  The solution count includes all sign choices.
 * The code enumerates non‑negative values of a,b,c,d and multiplies
 * by 2 for every non‑zero variable to obtain the full number of
 * integer solutions.  It uses an integer‐square‑root helper that
 * operates purely on 64‑bit unsigned integers, so no external
 * utilities are required.
 */
#include <stdlib.h>
#include <stddef.h>

/* --------------------------- integer square root --------------------------- */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n == 0 || n == 1)
        return n;
    idx_t x = n;
    idx_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

/* ------------------- cached representations of a^2 + b^2 ------------------- */
static seq_t *rep_ab_cache = NULL;   /* rep_ab_cache[t] = #solutions for a^2 + b^2 = t */
static idx_t rep_ab_size = 0;        /* current cache size (elements 0 .. rep_ab_size-1) */

static void ensure_rep_ab(idx_t n)
{
    if (rep_ab_size > n)
        return;                       /* already have required range */

    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(rep_ab_cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        abort();                      /* out of memory */
    }

    for (idx_t t = rep_ab_size; t <= n; ++t) {
        seq_t count = 0;
        idx_t max_a = isqrt_u64(t);
        for (idx_t a = 0; a <= max_a; ++a) {
            idx_t a2 = a * a;
            idx_t rem = t - a2;
            idx_t b = isqrt_u64(rem);
            if (b * b != rem)
                continue;              /* not a perfect square */

            idx_t mult_a = (a == 0) ? 1 : 2;
            idx_t mult_b = (b == 0) ? 1 : 2;
            count += (seq_t)mult_a * (seq_t)mult_b;
        }
        new_cache[t] = count;
    }

    rep_ab_cache = new_cache;
    rep_ab_size  = new_size;
}

/* --------------------------- main sequence function ----------------------- */
seq_t a(idx_t n)
{
    /* We need rep_ab_cache up to n because the largest remainder
       after removing 2*c^2 and 11*d^2 is at most n. */
    ensure_rep_ab(n);

    seq_t total = 0;

    idx_t max_d = isqrt_u64(n / 11);
    for (idx_t d = 0; d <= max_d; ++d) {
        idx_t d2   = d * d;
        idx_t rem_d  = n - 11 * d2;            /* remaining part for a^2 + b^2 + 2*c^2 */
        if (rem_d < 0)
            continue;                          /* should not happen because of max_d bound */

        idx_t mult_d = (d == 0) ? 1 : 2;

        idx_t max_c = isqrt_u64(rem_d / 2);
        seq_t subtotal = 0;
        for (idx_t c = 0; c <= max_c; ++c) {
            idx_t c2   = c * c;
            idx_t rem_c  = rem_d - 2 * c2;     /* remaining part for a^2 + b^2 */
            if (rem_c < 0)
                continue;                      /* not needed due to max_c bound */

            idx_t mult_c = (c == 0) ? 1 : 2;
            subtotal += rep_ab_cache[rem_c] * mult_c;
        }

        total += subtotal * mult_d;
    }

    return total;
}
