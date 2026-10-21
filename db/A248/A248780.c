/*
 * A248780  Number of cubes that divide n!
 * https://oeis.org/A248780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{p^e || n!} (1 + floor(e/3))
 * Range:     u8 n <= 27, u16 n <= 62, u32 n <= 146, u64 n <= 353,
 *            u128 n <= 848, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* counts the number of cube‑divisors of n! */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* dynamic prime table */
static idx_t *prime_list = NULL;           /* array of primes found so far   */
static size_t prime_cnt  = 0;              /* number of stored primes       */
static size_t prime_cap  = 0;              /* current buffer capacity       */

/* ensure there is space for at least 'cap' primes                                 */
static void ensure_prime_cap(size_t cap)
{
    if (prime_cap >= cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 128;
    if (new_cap < cap) new_cap = cap;
    if (prime_list == NULL) {
        prime_list = (idx_t*)malloc(new_cap * sizeof(idx_t));
    } else {
        prime_list = (idx_t*)realloc(prime_list, new_cap * sizeof(idx_t));
    }
    prime_cap = new_cap;
}

/* generate primes up to at least 'limit' (inclusive) if not already done.       */
static void ensure_primes_up_to(idx_t limit)
{
    if (prime_cnt && prime_list[prime_cnt - 1] >= limit) return;

    /* start from the first unseen number */
    idx_t start = prime_cnt ? prime_list[prime_cnt - 1] + 1 : 2;

    for (idx_t n = start; n <= limit; ++n) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if (p * p > n) break;
            if (n % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            ensure_prime_cap(prime_cnt + 1);
            prime_list[prime_cnt++] = n;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n==0, but safe fallback */

    /* make sure all primes up to n are available */
    ensure_primes_up_to(n);

    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= n; ++i) {
        idx_t p = prime_list[i];
        idx_t e = 0;
        idx_t pow = p;
        while (pow <= n) {
            e += n / pow;
            if (pow > n / p) break;      /* avoid overflow */
            pow *= p;
        }
        result *= (seq_t)(e / 3 + 1);
    }
    return result;
}
