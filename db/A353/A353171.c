/*
 * A353171  Irregular triangle read by rows; T(n,k) = 2^k (mod prime(n)),
 *          terminating when T(n,k) = 1.
 * https://oeis.org/A353171
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, look, tabf
 * Formula:   not stated
 * Range:     i8 n <= 3572, i16 n <= 9646, i32 n <= 9646, i64 n <= 9646,
 *            i128 n <= 9646, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A353171: Irregular triangle read by rows.
   For each odd prime p (starting with 3), list the residues of successive
   powers of 2 modulo p:
        r_k = 2^k mod p   (k = 1,2,…)
   Replace each r_k by its signed least‑significant residue:
        val = r_k          if r_k ≤ p/2
        val = r_k - p      otherwise
   Stop the row when r_k == 1.  Concatenate all rows in order.
   The sequence a(n) (offset 2) is the n‑th element of this concatenation. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static uint64_t *primes = NULL;   /* stored primes, ascending  (prime[0]=2) */
static size_t primes_count = 0;   /* number of primes stored */
static size_t primes_cap = 0;     /* current allocation size */
static uint64_t primes_last = 0;  /* last prime in the array */

/* initialise prime list with first prime 2 */
static void init_primes(void)
{
    if (primes_count != 0) return;
    primes_cap = 16;
    primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) abort();
    primes[0] = 2;
    primes_count = 1;
    primes_last = 2;
}

/* append the next prime after primes_last to the list */
static void append_next_prime(void)
{
    uint64_t cand = primes_last + 1;
    while (1) {
        uint64_t limit = (uint64_t)sqrt((double)cand);
        bool is_prime = true;
        for (size_t i = 0; i < primes_count && primes[i] <= limit; ++i) {
            if (cand % primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) break;
        ++cand;
    }

    if (primes_count >= primes_cap) {
        size_t newcap = primes_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = newcap;
    }

    primes[primes_count++] = cand;
    primes_last = cand;
}

/* ensure that prime list has at least (needed+1) primes (index needed exists) */
static void ensure_prime_count(size_t needed)
{
    while (primes_count <= needed) append_next_prime();
}

/* compute a(n); n >= 2 is defined, other values return 0 */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;  /* undefined below offset */

    init_primes();  /* guarantee primes[0] = 2 is ready */

    idx_t term_index = 2;   /* current term index (offset 2) */
    size_t prime_idx = 1;   /* start with prime(2) = 3 (odd primes only) */

    while (true) {
        ensure_prime_count(prime_idx);
        uint64_t p = primes[prime_idx];

        uint64_t r = 2 % p;  /* 2^1 mod p */
        while (true) {
            seq_t val = (seq_t)r;
            if (r > p / 2) val -= (seq_t)p;

            if (term_index == n) return val;

            ++term_index;
            if (r == 1) break;          /* end of the current row */
            r = (r * 2) % p;            /* next power of 2 modulo p */
        }
        ++prime_idx;  /* move to the next prime for the next row */
    }
}
