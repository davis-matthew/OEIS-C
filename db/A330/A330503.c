/*
 * A330503  Number of Sós permutations of {0,1,...,n}.
 * https://oeis.org/A330503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1) * ∑_{k=1..n} φ(k)
 * Range:     u8 n <= 8, u16 n <= 59, u32 n <= 2416, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *phi_prefix = NULL;
static idx_t phi_prefix_max = 0;

static void ensure_phi_prefix(idx_t n)
{
    if (n <= phi_prefix_max)
        return;

    idx_t new_max = n;
    /* Temporary array holding φ values */
    seq_t *phi = (seq_t *)malloc(sizeof(seq_t) * (new_max + 1));
    for (idx_t i = 0; i <= new_max; ++i)
        phi[i] = (seq_t)i;

    /* Euler's totient sieve */
    for (idx_t i = 2; i <= new_max; ++i) {
        if (phi[i] == (seq_t)i) {  /* i is prime */
            for (idx_t j = i; j <= new_max; j += i)
                phi[j] -= phi[j] / (seq_t)i;
        }
    }

    /* Prefix sums of φ */
    seq_t *new_prefix = (seq_t *)malloc(sizeof(seq_t) * (new_max + 1));
    new_prefix[0] = 0;
    for (idx_t i = 1; i <= new_max; ++i)
        new_prefix[i] = new_prefix[i - 1] + phi[i];

    free(phi);
    if (phi_prefix)
        free(phi_prefix);
    phi_prefix = new_prefix;
    phi_prefix_max = new_max;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* sequence offset is 1 */

    ensure_phi_prefix(n);

    seq_t sum_phi = phi_prefix[n];
    seq_t result = (seq_t)(n + 1) * sum_phi;
    return result;
}
