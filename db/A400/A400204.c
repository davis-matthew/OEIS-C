/*
 * A400204  Minimum perimeter of a right-angled triangle with integer sides
 *          whose area is divisible by the n-th prime.
 * https://oeis.org/A400204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12 for n = 1,2 a(3) = 30 a(4) = 56 a(n) = 12 * p_n for n
 *            >= 5 where p_n is the n‑th prime
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cached prime table */
static uint64_t *prime_tbl = NULL;   /* array of primes */
static size_t prime_cnt   = 0;       /* number of primes stored */
static size_t prime_cap   = 0;       /* capacity of prime_tbl */
static uint64_t next_cand = 3;      /* next odd number to test */

/* Ensure that the table contains at least n primes. */
static void
ensure_primes(size_t n)
{
    if (prime_tbl == NULL) {                /* first time: initialise */
        prime_cap = 16;
        prime_tbl = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_tbl) return;             /* out of memory – should not happen */
        /* The first prime is 2 */
        prime_tbl[0] = 2;
        prime_cnt   = 1;
        next_cand   = 3;
    }

    while (prime_cnt < n) {
        uint64_t cand = next_cand;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_tbl[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap << 1;
                if (new_cap == 0) new_cap = 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_tbl, new_cap * sizeof(uint64_t));
                if (!tmp) return;           /* allocation failed – abort */
                prime_tbl = tmp;
                prime_cap = new_cap;
            }
            prime_tbl[prime_cnt++] = cand;
        }
        next_cand += 2;                    /* skip even numbers */
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined, but avoid crash */

    if (n == 1 || n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)30;
    if (n == 4) return (seq_t)56;

    /* For n >= 5 use 12 times the n-th prime */
    size_t nn = (size_t)n;                   /* safe cast: n >= 5 fits in size_t */
    ensure_primes(nn);
    seq_t p = (seq_t)prime_tbl[nn - 1];       /* n-th prime is at index n-1 */
    return 12 * p;
}
