/*
 * A360883  Smallest powerful (1) number which is at the end of an arithmetic
 *          progression of n terms.
 * https://oeis.org/A360883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A360883 (from the OEIS entry):
   a(1) = 1,
   a(2) = 4,
   a(3) = 49,
   for n ≥ 4:  a(n) = n × (∏_{p ≤ n} p)^2
   i.e. the product of all primes ≤ n, squared, times n.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Maximum n that may occur in this problem. */
#define MAX_N 10000

/* Prime sieve: is_prime[i] is true if i is prime. */
static bool is_prime[MAX_N + 1];

/* Initialization flag.  */
static bool initialized = false;

/* Perform sieve once and store primality. */
static void init_sieve(void)
{
    if (initialized) return;

    for (int i = 0; i <= MAX_N; ++i) is_prime[i] = false;
    for (int i = 2; i <= MAX_N; ++i) is_prime[i] = true;

    for (int p = 2; p * p <= MAX_N; ++p) {
        if (is_prime[p]) {
            for (int q = p * p; q <= MAX_N; q += p) {
                is_prime[q] = false;
            }
        }
    }
    initialized = true;
}

/* Main function requested by the problem. */
seq_t a(idx_t n)
{
    init_sieve();

    /* Handle the special small cases. */
    if (n <= 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)49;

    /* Compute the primorial: product of all primes ≤ n. */
    seq_t primorial = (seq_t)1;
    for (int i = 2; i <= (int)n; ++i) {
        if (is_prime[i]) {
            primorial = primorial * (seq_t)i;
        }
    }

    /* a(n) = n × (primorial)^2. */
    seq_t result = primorial * primorial;
    result = result * (seq_t)n;
    return result;
}
