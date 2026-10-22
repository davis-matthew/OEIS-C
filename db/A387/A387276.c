/*
 * A387276  a(n) = Sum_{k=0..n} 3^(n-k) * binomial(n+3,k+3) *
 *          binomial(2*k+6,k+6).
 * https://oeis.org/A387276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^(n-k) * binomial(n+3,k+3) *
 *            binomial(2*k+6,k+6) Recurrence for n > 1: n*(n+6)*a(n) =
 *            (n+3)*(5*(2*n+5)*a(n-1) - 21*(n+2)*a(n-2)) Initial values:
 *            a(0)=1, a(1)=20
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array of computed terms */
static size_t cache_size = 0;     /* number of terms stored (valid indices 0..cache_size-1) */
static size_t cache_cap  = 0;     /* allocated capacity */

/* Helper: ensure enough capacity to store indices up to n */
static void ensure_capacity_for(idx_t n)
{
    if (n < cache_size) return;          /* nothing to do */

    size_t new_cap = cache_cap ? cache_cap : 2;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);          /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Main function */
seq_t a(idx_t n)
{
    /* compute missing terms up to n */
    ensure_capacity_for(n);

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)20;
        } else {
            /* Use different paths depending on the size of seq_t */
            if (sizeof(seq_t) <= sizeof(unsigned long long)) {
                /* For builtin integer types (8, 16, 32, 64 bits) use 128‑bit intermediate */
                unsigned __int128 a_im1 = (unsigned __int128)cache[i-1];
                unsigned __int128 a_im2 = (unsigned __int128)cache[i-2];
                unsigned __int128 term_left  = (unsigned __int128)5 * (unsigned __int128)(2*i+5) * a_im1;
                unsigned __int128 term_right = (unsigned __int128)21 * (unsigned __int128)(i+2) * a_im2;
                unsigned __int128 numerator   = (unsigned __int128)(i+3) * (term_left - term_right);
                unsigned __int128 denominator = (unsigned __int128)i * (unsigned __int128)(i+6);
                cache[i] = (seq_t)(numerator / denominator);
            } else {
                /* For arbitrary‑precision or 128‑bit seq_t use pure seq_t operations */
                seq_t coeff1 = (seq_t)((unsigned long long)(5 * (2*i+5)));
                seq_t coeff2 = (seq_t)((unsigned long long)(21 * (i+2)));
                seq_t term_left  = coeff1 * cache[i-1];
                seq_t term_right = coeff2 * cache[i-2];
                seq_t numerator   = (seq_t)(i+3) * (term_left - term_right);
                seq_t denominator = (seq_t)i * (seq_t)(i+6);
                cache[i] = numerator / denominator;   /* exact division */
            }
        }
    }
    cache_size = (size_t)n + 1;   /* all terms up to n are now stored */
    return cache[n];
}
