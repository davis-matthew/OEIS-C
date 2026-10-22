/*
 * A346622  a(n) = card{ x <= n : x odd and omega(x) = 2 }.
 * https://oeis.org/A346622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { x ≤ n : x odd ∧ ω(x) = 2 }
 * Range:     u8 n <= 980, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* PRIVATE state shared by all calls. */
static unsigned char *omega = NULL;   /* number of distinct prime factors */
static uint64_t      *pref  = NULL;   /* cumulative counts for odd numbers */
static idx_t         max_n = 0;       /* largest n that has been sieved */

/* Ensure the tables are valid up to at least n. */
static void ensure(idx_t n)
{
    if (n <= max_n)
        return;

    /* Reallocate arrays to size n+1 (indexes 0 … n). */
    omega = (unsigned char *)realloc(omega, (size_t)(n + 1) * sizeof(unsigned char));
    if (!omega) abort();
    pref  = (uint64_t *)realloc(pref,  (size_t)(n + 1) * sizeof(uint64_t));
    if (!pref) abort();

    /* Initialise the whole array to zero. */
    for (idx_t i = 0; i <= n; ++i)
        omega[i] = 0;

    /* Sieve ω (number of distinct prime factors). */
    for (idx_t p = 2; p <= n; ++p) {
        if (omega[p] == 0) {            /* p is prime */
            for (idx_t j = p; j <= n; j += p)
                ++omega[j];
        }
    }

    /* Build the odd‑only prefix sum. */
    uint64_t acc = 0;
    pref[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if ((i & 1U) && omega[i] == 2)
            ++acc;
        pref[i] = acc;
    }

    max_n = n;
}

/* Public sequence entry function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    ensure(n);
    return (seq_t)pref[n];
}
