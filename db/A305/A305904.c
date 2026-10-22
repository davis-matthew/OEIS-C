/*
 * A305904  Filter sequence for all such sequences S, for which S(A091206(k))
 *          = constant for all k >= 3, where A091206 gives primes whose binary
 *          representation encodes a polynomial irreducible over GF(2).
 * https://oeis.org/A305904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for 1 ≤ n < 7 a(n) = 7 if n is a prime whose binary
 *            representation encodes an irreducible polynomial over GF(2) a(n)
 *            = n - C(n) + 1 otherwise, where C(n) = number of such primes ≤
 *            n. This implementation builds a cache of the qualifying primes
 *            up to the largest argument seen and uses that...
 * Range:     u8 n <= 279, u16 n <= 9979, u32 n <= 9979, u64 n <= 9979,
 *            u128 n <= 9979, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer floor of log₂(x).  Returns -1 for x==0. */
static inline int floor_log2_uint64(uint64_t v)
{
    return v ? (63 - __builtin_clzll(v)) : -1;
}

/* simple deterministic primality test for n ≤ 10 000. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if ((n & 1ULL) == 0) return n == 2;
    if (n % 3ULL == 0) return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* returns true iff g divides f when polynomials are taken over GF(2). */
static bool divides(uint64_t f, uint64_t g)
{
    if (g == 0) return false;
    while (f) {
        int deg_f = floor_log2_uint64(f);
        int deg_g = floor_log2_uint64(g);
        if (deg_f < deg_g) break;
        f ^= g << (deg_f - deg_g);
    }
    return f == 0;
}

/* test whether the binary representation of prime p encodes an irreducible
   polynomial over GF(2) (degree ≥ 2). */
static bool is_irreducible(uint64_t p)
{
    int d = floor_log2_uint64(p);
    if (d <= 1) return false;          /* degree must be at least 2 */

    int maxdg = d / 2;                 /* test factors up to half the degree */
    for (int dg = 1; dg <= maxdg; ++dg) {
        uint64_t start = 1ULL << dg;   /* leading coefficient must be 1 */
        uint64_t end   = 1ULL << (dg + 1);
        for (uint64_t g = start; g < end; ++g) {
            if (divides(p, g)) return false;   /* reducible */
        }
    }
    return true;                       /* no factor found */
}

/* true if n is a qualifying prime (≥7 and irreducible binary polynomial). */
static bool is_special_prime(uint64_t n)
{
    if (n < 7) return false;
    if (!is_prime_uint64(n)) return false;
    return is_irreducible(n);
}

/* cache of all qualifying primes seen so far. */
static uint64_t *special_vals   = NULL;
static size_t    special_count  = 0;
static size_t    special_cap    = 0;
static idx_t     last_checked   = 0;

seq_t a(idx_t n)
{
    if (n < 7) return (seq_t)n;

    /* Extend the cache up to n. */
    if (n > last_checked) {
        for (idx_t i = last_checked + 1; i <= n; ++i) {
            if (is_special_prime((uint64_t)i)) {
                if (special_count == special_cap) {
                    size_t new_cap = special_cap ? special_cap * 2 : 16;
                    uint64_t *tmp = (uint64_t *)realloc(special_vals,
                                                         new_cap * sizeof(uint64_t));
                    if (!tmp) exit(1);                  /* allocation failure */
                    special_vals = tmp;
                    special_cap  = new_cap;
                }
                special_vals[special_count++] = (uint64_t)i;
            }
        }
        last_checked = n;
    }

    /* If n itself is a qualifying prime, the value is 7. */
    if (is_special_prime((uint64_t)n)) return (seq_t)7;

    /* Otherwise a(n) = n - C(n) + 1, where C(n) = special_count. */
    return (seq_t)(n - special_count + 1);
}
