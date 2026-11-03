/*
 * A276731  Number of solutions to y^2 + y == x^3 - 7 (mod p) as p runs
 *          through the primes.
 * https://oeis.org/A276731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6501, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9944 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n): number of solutions (x,y) modulo prime(n) of
 *  y^2 + y ≡ x^3 – 7 (mod prime(n)).  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */

    uint32_t p = get_prime(n);               /* prime(n) */
    if (p == 2U) {                           /* special handling for p = 2 */
        /* Brute‑force the 4 residue classes */
        seq_t cnt = 0;
        for (uint32_t x = 0; x < 2; ++x)
            for (uint32_t y = 0; y < 2; ++y)
                if (((y * y + y) % 2ULL) == ((x * x * x - 7ULL) % 2ULL))
                    ++cnt;
        return cnt;
    }

    /* For odd primes use quadratic‑residue test */
    seq_t count = 0;
    uint64_t mod = (uint64_t)p;

    for (uint32_t x = 0; x < p; ++x) {
        /* x^3 mod p */
        uint64_t xx = (uint64_t)x * x % mod;      /* x^2 mod p */
        uint64_t xxx = xx * x % mod;              /* x^3 mod p */

        /* RHS = x^3 – 7 (mod p) */
        uint64_t rhs = (xxx + mod - (7 % mod)) % mod;

        /* Discriminant Δ = 1 + 4·rhs (mod p) */
        uint64_t D = (1 + 4 * rhs) % mod;

        /* Legendre symbol (D|p) via Jacobi (p is prime) */
        int j = jacobi_symbol(D, mod);

        if (j == 1)          /* two y solutions */
            count += (seq_t)2;
        else if (j == 0)     /* one y solution when Δ ≡ 0 */
            count += (seq_t)1;
    }
    return count;
}
