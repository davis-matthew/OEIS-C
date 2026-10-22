/*
 * A354545  Expansion of e.g.f. exp(x)^( cos(x) + sin(x) ).
 * https://oeis.org/A354545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 16, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1; a(n) = Sum_{k=1..n} (-1)^floor((k-1)/2) * k *
 *                   binomial(n-1,k-1) * a(n-k)
 */
#include <stdlib.h>

/* maximum precomputation limit (should cover the expected n range) */
#define MAX_N 20000

static seq_t a_cache[MAX_N + 1];
static seq_t binom[MAX_N + 1];
static size_t a_len = 1;       /* next free index in a_cache */
static size_t binom_len = 1;   /* length of the current binomial row */
static int init_done = 0;

/* one‑time initialization */
static void init(void)
{
    if (init_done) return;
    a_cache[0] = (seq_t)1;
    binom[0] = (seq_t)1;
    init_done = 1;
}

/* extend the caches up to index n (inclusive) */
static void ensure_a_size(idx_t n)
{
    if (n < a_len) return;
    if (n > MAX_N) return;   /* out of bound – caller should avoid this */
    while (a_len <= n)
    {
        size_t next = a_len;   /* computing a[next] */

        /* compute the sum */
        seq_t sum = 0;
        for (size_t k = 1; k <= next; ++k)
        {
            /* sign: + for floor((k-1)/2) even, - for odd */
            int sign = (((k - 1) / 2) & 1) ? -1 : 1;

            /* binomial coefficient: binom[next-1][k-1] stored in binom[k-1] */
            seq_t term = binom[k - 1];

            /* multiply by a[next-k] */
            term *= a_cache[next - k];

            /* multiply by k */
            term *= (seq_t)k;

            /* apply sign */
            if (sign == -1) term = -term;

            /* accumulate */
            sum += term;
        }
        a_cache[next] = sum;
        ++a_len;

        /* Update binomial row to the next (next) row */
        /* Ensure the array has space for index 'next' */
        binom[next] = 0;
        for (size_t j = next; j > 0; --j)
        {
            /* binom[j] = binom_prev[j] + binom_prev[j-1] */
            binom[j] = binom[j] + binom[j - 1];
        }
        ++binom_len;   /* now binom_len == next + 1 */
    }
}

/* entry point required by the build */
seq_t a(idx_t n)
{
    init();
    if (n > MAX_N) return (seq_t)0;   /* safety guard */
    ensure_a_size(n);
    return a_cache[n];
}
