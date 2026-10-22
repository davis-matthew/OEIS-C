/*
 * A330292  a(n) = number of integers 1 <= k < n such that omega(k) <=
 *          omega(n), where omega = A001221.
 * https://oeis.org/A330292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{ k : 1 ≤ k < n and ω(k) ≤ ω(n) }|
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ω(k) = number of distinct prime factors of k (A001221) */

#include <stdlib.h>
#include <stdint.h>

/* static state for the sieve */
static idx_t      max_sieve = 0;   /* largest n that has been sieved   */
static uint8_t   *omega_arr = NULL;/* ω(i) for i in [0, max_sieve]      */

/* Ensure ω values available up to at least n */
static void
ensure_sieve(idx_t n)
{
    if (n <= max_sieve)          /* already computed */
        return;

    /* allocate an array 0 .. n (size n+1) */
    size_t new_size = (size_t)n + 1;
    uint8_t *new_omega = (uint8_t *)calloc(new_size, sizeof(uint8_t));
    if (!new_omega)
        exit(EXIT_FAILURE);      /* allocation failed */

    /* sieve: for each prime i, increment ω[j] for all multiples j of i */
    for (idx_t i = 2; i <= n; ++i) {
        if (new_omega[i] == 0) {                 /* i is prime */
            for (idx_t j = i; j <= n; j += i)
                ++new_omega[j];
        }
    }

    /* discard old data and adopt the new array */
    free(omega_arr);
    omega_arr = new_omega;
    max_sieve = n;
}

/* OEIS sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)                    /* a(1) = 0, no lower indices */
        return (seq_t)0;

    ensure_sieve(n);               /* guarantee ω[0..n] are ready */

    uint8_t omega_n = omega_arr[n];
    seq_t count = (seq_t)0;

    for (idx_t k = 1; k < n; ++k) {
        if (omega_arr[k] <= omega_n)
            ++count;
    }
    return count;
}
