/*
 * A282727  Let p = n-th prime == 3 mod 8; a(n) = (sum of quadratic residues
 *          mod p that are < p/2) + (sum of all quadratic residues mod p).
 * https://oeis.org/A282727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all quadratic residues mod p + * * sum of those
 *            residues < p/2 * * where p = n‑th prime ≡ 3 (mod 8)
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 2782, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

/* ------------------------------------------------------------ */
/* Static storage for primes ≡ 3 (mod 8) */
static uint64_t *prime3      = NULL; /* array of primes 3 mod 8   */
static size_t   prime3cnt    = 0;    /* number stored              */
static size_t   prime3cap    = 0;    /* current capacity           */

/* Add a new prime to the static array, grow if necessary */
static void add_prime(uint64_t p)
{
    if (prime3cnt >= prime3cap) {
        prime3cap = (prime3cap == 0) ? 16 : prime3cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(prime3, prime3cap * sizeof(uint64_t));
        prime3 = tmp;                           /* build harness guarantees success   */
    }
    prime3[prime3cnt++] = p;
}

/* Ensure the array contains at least n primes 3 mod 8       */
static void ensure_prime3_n(size_t n)
{
    uint64_t cand = (prime3cnt == 0) ? 3ULL : prime3[prime3cnt - 1] + 8ULL;
    while (prime3cnt < n) {
        if (is_prime((idx_t)cand)) {
            add_prime(cand);
        }
        cand += 8ULL;                          /* next candidate 3 (mod 8)   */
    }
}

/* ------------------------------------------------------------ */
/* Main sequence function                                       */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* outside the OEIS offset */

    /* Get the n‑th prime p ≡ 3 (mod 8) */
    ensure_prime3_n((size_t)n);
    seq_t p = (seq_t)prime3[n - 1];

    /* Sum of all quadratic residues and of those < p/2 */
    seq_t sum_all   = 0;
    seq_t sum_lower = 0;

    uint64_t p64    = (uint64_t)p;
    uint64_t half   = p64 / 2;
    idx_t    kmax   = (idx_t)((p64 - 1) / 2);   /* (p-1)/2 works for 0 < p  */

    for (idx_t k = 1; k <= kmax; ++k) {
        uint64_t kk   = (uint64_t)k * (uint64_t)k;
        uint64_t r64  = kk % p64;               /* quadratic residue          */
        seq_t r      = (seq_t)r64;

        sum_all += r;
        if ( (r64 << 1) < p64 )                /* r < p/2, avoid double cast   */
            sum_lower += r;
    }

    return sum_all + sum_lower;
}
