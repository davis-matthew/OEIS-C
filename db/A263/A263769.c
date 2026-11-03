/*
 * A263769  Smallest prime q such that q == -1 (mod prime(n)-1).
 * https://oeis.org/A263769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime q such that q ≡ -1 (mod prime(n)-1)
 * Range:     u8 n <= 23, u16 n <= 612, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Validated: standard, pass, 64 terms through n = 5547 (2026-11-03)
 * Helpers:   gcd, is_prime_u64, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/isqrt.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* static cache of primes (1‑based: primes[0] = 2) */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_capacity = 0;

/* ensure at least n primes are stored in prime_list */
static void ensure_n_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;

    /* start searching from next integer after last prime (or 2) */
    uint64_t candidate = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;

    while (prime_count < (size_t)n) {
        uint64_t limit = (uint64_t)isqrt(candidate);
        bool prime = true;
        for (size_t i = 0; i < prime_count && prime_list[i] <= limit; ++i) {
            if (candidate % prime_list[i] == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            /* grow array if necessary */
            if (prime_count == prime_capacity) {
                size_t new_cap = (prime_capacity == 0) ? 128 : prime_capacity * 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                prime_list = tmp;
                prime_capacity = new_cap;
            }
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* return the n-th prime (1‑based) */
static inline uint64_t nth_prime(idx_t n)
{
    ensure_n_primes(n);
    return prime_list[(size_t)n - 1];
}

/* Main function requested by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)     /* behaviour below offset is irrelevant */
        return (seq_t)0;

    uint64_t p = nth_prime(n);    /* n-th prime */
    uint64_t m = p - 1;           /* modulus */
    uint64_t r = (m == 0) ? 0 : m - 1;  /* residue ≡ -1 mod m */

    /* start with the smallest non‑negative integer ≡ r (mod m) */
    uint64_t q = r;

    /* shift upwards until q ≥ 2 */
    if (q < 2) {
        uint64_t k = (2 - q + m - 1) / m;  /* ceil((2-q)/m) */
        q += k * m;
    }

    /* search forward in the arithmetic progression until a prime is found */
    while (!is_prime_big((seq_t)q))
        q += m;

    return (seq_t)q;
}
