/*
 * A370763  Table read by rows: row n is the unique primitive Pythagorean
 *          triple (a,b,c) such that a = prime(n)*prime(n+1) and its long leg
 *          and hypotenuse are consecutive natural numbers.
 * https://oeis.org/A370763
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   Let k = (n-2)/3 + 2 Let r = (n-2) mod 3 Let pₖ be the k‑th
 *            prime, qₖ₊₁ the (k+1)‑th prime. Then a(n) = { pₖ * qₖ₊₁ , r == 0
 *            (pₖ² * qₖ₊₁² - 1) / 2 , r == 1 (pₖ² * qₖ₊₁² + 1) / 2 , r == 2 }
 *            Offset: 2 (The sequence lists the components of the primitive
 *            Pythagorean triples whose short leg equals pₖ·q...
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Dynamic array of primes (1‑indexed: primes[0] = 2) */
static uint64_t *primes = NULL;   /* stored primes */
static size_t   prime_cnt = 0;    /* number of primes stored */
static size_t   prime_cap = 0;    /* allocated capacity */

/* Ensure at least (count_needed) primes are available (1‑indexed). */
static void ensure_primes(size_t count_needed)
{
    if (prime_cnt > count_needed) return;   /* already enough */

    /* Grow the array if necessary */
    if (prime_cap < count_needed + 10) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 32;
        while (new_cap < count_needed + 10) new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) return;                  /* allocation failed – ignore */
        primes = tmp;
        prime_cap = new_cap;
    }

    uint64_t candidate = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1;

    while (prime_cnt <= count_needed) {
        /* Handle the first two primes directly */
        if (candidate == 2 || candidate == 3) {
            primes[prime_cnt++] = candidate;
            candidate++;
            continue;
        }

        /* Skip even numbers */
        if ((candidate & 1) == 0) {
            candidate++;
            continue;
        }

        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            primes[prime_cnt++] = candidate;
        }
        candidate++;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* undefined for n < 2, return 0 */

    /* Determine row index k and position pos */
    idx_t k   = (n - 2) / 3 + 2;      /* 1‑based index of the first prime in the row */
    idx_t pos = (n - 2) % 3;          /* 0: short leg, 1: long leg, 2: hypotenuse */

    /* Obtain the required primes */
    ensure_primes((size_t)k + 1);      /* need primes up to the (k+1)-th */
    uint64_t p = primes[k - 1];        /* p = prime(k)   */
    uint64_t q = primes[k];            /* q = prime(k+1) */

    /* Compute values */
    seq_t pp   = (seq_t)p * (seq_t)p;  /* p² */
    seq_t qq   = (seq_t)q * (seq_t)q;  /* q² */
    seq_t prod = pp * qq;              /* (p·q)² */

    if (pos == 0)          /* short leg a = p·q */
        return (seq_t)p * (seq_t)q;
    else if (pos == 1)     /* long leg b = (p²·q² – 1)/2 */
        return (prod - 1) / 2;
    else                   /* hypotenuse c = (p²·q² + 1)/2 */
        return (prod + 1) / 2;
}
