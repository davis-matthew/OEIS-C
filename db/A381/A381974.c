/*
 * A381974  Primes of the form Sum_{k >= 0} floor(m/3^k) for some number m.
 * https://oeis.org/A381974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   p is a prime if there exists an integer m such that p = Σ_{k≥0}
 *            floor(m/3^k). The sequence a(n) lists the primes p in ascending
 *            order, indexed with a(1)=2
 * Range:     u8 n <= 36, u16 n <= 3776, u32 n <= 4268, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *prime_vals = NULL;   /* array of found primes */
static size_t prime_count = 0;     /* number of primes stored */
static size_t prime_cap = 0;       /* allocated capacity of prime_vals */
static idx_t m_next = 1;           /* next m to test */

/* Ensure at least 'need' slots are available in prime_vals. */
static void ensure_prime_cap(size_t need)
{
    if (prime_cap >= need)
        return;
    size_t newcap = prime_cap ? prime_cap << 1 : 64;
    while (newcap < need)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prime_vals, newcap * sizeof(seq_t));
    if (!tmp) {
        /* memory allocation failed – abort as per limits of the exercise */
        abort();
    }
    prime_vals = tmp;
    prime_cap = newcap;
}

/* Compute a(n): n ≥ 1.  Calls are allowed in any order and the function
   remembers previously computed primes. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* index 0 is not defined in OEIS, returning 0 is harmless */

    if ((size_t)n <= prime_count)
        return prime_vals[n - 1];

    /* generate primes until we have enough */
    for (; prime_count < (size_t)n; ++m_next) {
        seq_t t = (seq_t)m_next;
        seq_t s = 0;
        while (t) {
            s += t;
            t /= 3;
        }
        if (is_prime_big(s)) {
            ensure_prime_cap(prime_count + 1);
            prime_vals[prime_count++] = s;
        }
    }
    return prime_vals[n - 1];
}
