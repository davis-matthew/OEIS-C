/*
 * A248170  Nonnegative integer whose square is the closest square to
 *          prime(n).
 * https://oeis.org/A248170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nearest integer k such that k² is the square closest
 *            to prime(n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


/*------------------------------------------------------------*/
/* Helper: floor(sqrt(x)) for 64‑bit integers                 */
/*------------------------------------------------------------*/
static inline uint64_t floor_sqrt_u64(uint64_t x)
{
    uint64_t result = 0;
    uint64_t bit = 1ULL << 62;           /* highest power of 4 <= 2⁶⁴   */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= result + bit) {
            x -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }
    return result;
}

/*------------------------------------------------------------*/
/* Prime cache support                                       */
/*------------------------------------------------------------*/
static uint32_t *prime_cache = NULL;   /* dynamic array of primes   */
static size_t   prime_cnt   = 0;       /* number of primes stored   */
static size_t   prime_cap   = 0;       /* allocated capacity        */

/* Ensure that at least n primes are stored in prime_cache.   */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;

    if (!prime_cache) {
        prime_cap   = 128;
        prime_cache = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
        if (!prime_cache) return;          /* allocation failure           */
        prime_cnt = 0;
    }

    uint32_t last = (prime_cnt ? prime_cache[prime_cnt-1] : 1);

    while (prime_cnt < (size_t)n) {
        last += 1;                          /* test the next integer       */
        bool is_prime = true;
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_cache[j];
            uint64_t p2 = (uint64_t)p * p;
            if (p2 > last) break;
            if (last % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                uint32_t *new_cache =
                    (uint32_t*)realloc(prime_cache, prime_cap * sizeof(uint32_t));
                if (!new_cache) return;      /* allocation failure           */
                prime_cache = new_cache;
            }
            prime_cache[prime_cnt++] = last;
        }
    }
}

/*------------------------------------------------------------*/
/* Main sequence function                                    */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset is 1, undefined below */

    ensure_primes(n);

    uint32_t p = prime_cache[n-1];       /* n‑th prime (0‑based index)  */
    uint64_t k = floor_sqrt_u64(p);
    uint64_t k2 = k * k;
    uint64_t d1 = p - k2;
    uint64_t k1 = k + 1;
    uint64_t k1sq = k1 * k1;
    uint64_t d2 = k1sq - p;

    uint64_t m = (d1 <= d2) ? k : k1;
    return (seq_t)m;
}
