/*
 * A331759  a(n) = A115004(2n+1).
 * https://oeis.org/A331759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 204, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (2*n+1)^2 + Sum_{i=2..2n+1} (2*n+2-i)*(4*n+4-i)*phi(i)
 *
 * Computes the Euler totient values as needed, caching them up to the
 * maximum required index to keep the function fast for repeated calls.
 */

#include <stdlib.h>   /* for malloc, free, calloc */

/* Forward declaration of phi cache data */
static seq_t *phi_cache = NULL;   /* array of phi values */
static idx_t   phi_cap   = 0;      /* current maximum index for which phi is computed */

static void ensure_phi_up_to(idx_t limit)
{
    if (phi_cap > limit)
        return;                    /* already computed enough */

    /* Recompute phi from scratch up to the new limit (simple and cheap). */
    seq_t *phi = (seq_t *)calloc(limit + 1, sizeof(seq_t));
    if (phi == NULL) exit(EXIT_FAILURE);   /* out of memory */

    /* Initialize phi[i] = i */
    for (idx_t i = 0; i <= limit; ++i)
        phi[i] = (seq_t)i;

    /* Euler's totient sieve */
    for (idx_t i = 2; i <= limit; ++i) {
        if (phi[i] == (seq_t)i) {            /* i is prime */
            for (idx_t j = i; j <= limit; j += i)
                phi[j] -= phi[j] / (seq_t)i;
        }
    }

    /* Replace old cache */
    free(phi_cache);
    phi_cache = phi;
    phi_cap   = limit;
}

seq_t a(idx_t n)
{
    /* Compute required max index for phi */
    idx_t max_i = 2 * n + 1;
    ensure_phi_up_to(max_i);

    /* Base term (2n+1)^2 */
    idx_t t = 2 * n + 1;
    seq_t a_val = (seq_t)t * (seq_t)t;

    /* Summation part */
    for (idx_t i = 2; i <= max_i; ++i) {
        seq_t f1 = (seq_t)(2 * n + 2 - i);
        seq_t f2 = (seq_t)(4 * n + 4 - i);
        seq_t term = f1 * f2;                  /* multiplication in seq_t */
        term *= phi_cache[i];                  /* multiply by phi(i) */
        a_val += term;
    }

    return a_val;
}
