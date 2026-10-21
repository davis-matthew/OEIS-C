/*
 * A256580  Number of quadruples (x, x+1, x+2, x+3) with 1 < x < p-3 of
 *          consecutive integers whose product is 1 mod p.
 * https://oeis.org/A256580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 168, u32 n <= 168, u64 n <= 168,
 *            u128 n <= 168, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A256580: a(n) = number of 1 < x < p_n-3 with x(x+1)(x+2)(x+3) ≡ 1 (mod p_n),
   where p_n is the n‑th prime.  The routine keeps a cache of primes and
   scans the interval for solutions.  It works for all integer types
   allowed by the build. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*--- prime cache -------------------------------------------------------*/
static uint64_t *prime_tbl = NULL;   /* array of found primes              */
static size_t prime_cnt  = 0;        /* number of primes stored            */
static size_t prime_cap  = 0;        /* allocated capacity                 */

/*--- grow the prime table until it contains at least n primes ---------*/
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)                /* already enough            */
        return;

    /* start searching from the next integer after the last prime    */
    uint64_t cand = (prime_cnt == 0) ? 2 : prime_tbl[prime_cnt - 1] + 1;

    /* enlarge table if necessary                                   */
    if (prime_cap < prime_cnt + 16) {
        size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_tbl,
                                            new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_tbl = tmp;
        prime_cap = new_cap;
    }

    /* trial‑division: add primes until we have n of them              */
    while (prime_cnt < (size_t)n) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_tbl[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            prime_tbl[prime_cnt++] = cand;
            if (prime_cnt == prime_cap) {           /* expand if full   */
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_tbl,
                                                new_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_tbl = tmp;
                prime_cap = new_cap;
            }
        }
        ++cand;
    }
}

/*--- main sequence function --------------------------------------------*/
seq_t a(idx_t n)
{
    /* safety for indices below the OEIS offset */
    if (n == 0) return 0;

    /* make sure the n‑th prime is computed */
    ensure_primes(n);
    uint64_t p = prime_tbl[n - 1];      /* p = n‑th prime */

    if (p <= 5)            /* interval (1, p‑3) is empty or too small          */
        return 0;

    seq_t count = 0;

    /* scan all admissible x : 2 <= x <= p-4 */
    for (idx_t x = 2; x + 3 < p; ++x) {
        seq_t prod = (seq_t)x;
        prod = (prod * (seq_t)(x + 1)) % (seq_t)p;
        prod = (prod * (seq_t)(x + 2)) % (seq_t)p;
        prod = (prod * (seq_t)(x + 3)) % (seq_t)p;
        if (prod == 1) ++count;
    }
    return count;
}
