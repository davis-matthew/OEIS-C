/*
 * A335187  Total area of all trapezoids with bases p+q, q-p and height p*q
 *          with p,q prime, n = p+q and p < q.
 * https://oeis.org/A335187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A335187: total area of all trapezoids with bases p+q, q‑p and height p*q,
   where n = p+q, p and q are distinct primes with p < q.
   Formula:  a(n) = Σ_{p+q=n, p<q, p,q primes} p*q^2  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static sieve state: isPrime[i] is true iff i is prime. */
    static bool *isPrime = NULL;
    static idx_t current_max_n = 0;

    /* Extend the sieve if needed. */
    if (n > current_max_n) {
        /* Allocate new sieve array.  Must cast malloc result in C++. */
        bool *new_sieve = (bool*)malloc((size_t)(n + 1) * sizeof(bool));
        if (!new_sieve) return (seq_t)0;   /* allocation failed */

        /* Initialize entries. */
        for (idx_t i = 0; i <= n; ++i) new_sieve[i] = true;
        if (n >= 0) new_sieve[0] = false;
        if (n >= 1) new_sieve[1] = false;

        /* Classic Eratosthenes sieve. */
        for (idx_t i = 2; i * i <= n; ++i) {
            if (new_sieve[i]) {
                for (idx_t j = i * i; j <= n; j += i)
                    new_sieve[j] = false;
            }
        }

        /* Release previous sieve (if any) and store the new one. */
        free(isPrime);
        isPrime    = new_sieve;
        current_max_n = n;
    }

    seq_t result = (seq_t)0;

    /* Iterate over potential primes p with 2 ≤ p ≤ n/2. */
    for (idx_t p = 2; p <= n / 2; ++p) {
        if (isPrime[p]) {
            idx_t q = n - p;
            if (q > p && isPrime[q]) {
                /* Compute p * q^2, casting to seq_t before multiplication
                   to avoid overflow in intermediate 64‑bit ints. */
                seq_t qp = (seq_t)q * (seq_t)q;      /* q^2 */
                seq_t term = (seq_t)p * qp;          /* p * q^2 */
                result += term;
            }
        }
    }

    return result;
}
