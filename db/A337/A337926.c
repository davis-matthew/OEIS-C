/*
 * A337926  Number of ways to write n as the sum of two positive integers with
 *          different numbers of distinct prime factors.
 * https://oeis.org/A337926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (1 - [omega(i)=omega(n-i)]) where
 *            omega(k) is the number of distinct prime factors of k
 * Range:     u8 n <= 734, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static unsigned char *omega_arr = NULL;   /* cache for omega values */
static idx_t          max_prepared = 0;   /* highest n for which omega_arr is valid */

/* Computes the number of ways to write n as a sum of two positive integers
   with different numbers of distinct prime factors. */
seq_t a(idx_t n)
{
    /* a(1) = a(2) = 0 by definition of the formula */
    if (n <= 2) return 0;

    /* Ensure omega_arr has been computed up to the required n. */
    if (omega_arr == NULL || n > max_prepared) {
        idx_t new_size = n;
        unsigned char *new_omega = (unsigned char*)malloc((new_size + 1) * sizeof(unsigned char));
        if (!new_omega) abort();                     /* out of memory */

        bool *is_composite = (bool*)malloc((new_size + 1) * sizeof(bool));
        if (!is_composite) abort();

        /* Initialize arrays */
        for (idx_t i = 0; i <= new_size; ++i) {
            new_omega[i] = 0;
            is_composite[i] = false;                /* all initially prime candidates */
        }
        new_omega[0] = new_omega[1] = 0;            /* omega(0)=omega(1)=0 */

        /* Simple sieve: for each prime p, increment omega[j] for all multiples j of p */
        for (idx_t p = 2; p <= new_size; ++p) {
            if (!is_composite[p]) {                 /* p is prime */
                for (idx_t j = p; j <= new_size; j += p) {
                    new_omega[j] += 1;
                    is_composite[j] = true;
                }
            }
        }
        free(is_composite);

        /* Replace old cache */
        free(omega_arr);
        omega_arr = new_omega;
        max_prepared = new_size;
    }

    /* Count unequal omega pairs */
    seq_t result = 0;
    idx_t half = n >> 1;
    for (idx_t i = 1; i <= half; ++i) {
        if (omega_arr[i] != omega_arr[n - i]) result++;
    }
    return result;
}
