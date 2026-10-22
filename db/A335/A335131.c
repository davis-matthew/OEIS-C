/*
 * A335131  a(n) = Sum_{k=1..n} phi(k)*phi(k+1)*phi(k+2), where phi(k) =
 *          A000010(k) is Euler's totient function.
 * https://oeis.org/A335131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} phi(k)*phi(k+1)*phi(k+2)
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static idx_t max_computed = 0;
static seq_t *phi_arr = NULL;
static seq_t *cum_arr = NULL;
static unsigned char *is_comp = NULL;

/* recompute phi[0..n+2] and cumulative sums a[1..n] when a larger n is requested */
static void recompute_upto(idx_t n)
{
    if (n <= max_computed)
        return;

    idx_t limit = n + 2;
    size_t sz = (size_t)(limit + 1);   /* indices 0..limit */

    seq_t *new_phi = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *new_cum = (seq_t *)malloc(sz * sizeof(seq_t));
    unsigned char *new_comp = (unsigned char *)malloc(sz);

    /* initialise phi[i] = i and composite flags to 0 */
    for (size_t i = 0; i < sz; ++i) {
        new_phi[i] = (seq_t)i;
        new_comp[i] = 0;
    }

    /* sieve to compute totients */
    for (idx_t p = 2; p <= limit; ++p) {
        if (!new_comp[p]) { /* p is prime */
            for (idx_t j = p; j <= limit; j += p) {
                new_phi[j] -= new_phi[j] / (seq_t)p;
                new_comp[j] = 1;          /* mark multiple as composite */
            }
        }
    }

    /* compute cumulative sums a[1..n] */
    new_cum[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t prod = new_phi[i] * new_phi[i + 1] * new_phi[i + 2];
        new_cum[i] = new_cum[i - 1] + prod;
    }

    /* free old buffers */
    free(phi_arr);
    free(cum_arr);
    free(is_comp);

    /* store new buffers */
    phi_arr = new_phi;
    cum_arr = new_cum;
    is_comp = new_comp;
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n < 1, but guard anyway */

    recompute_upto(n);
    return cum_arr[n];
}
