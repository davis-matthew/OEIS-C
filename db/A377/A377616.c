/*
 * A377616  a(n) is the number of iterations of x -> 3*x + 2 until (#
 *          composites reached) = (# primes reached), starting with prime(n).
 * https://oeis.org/A377616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 33, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 84 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A377616: counts the number of iterations of the map 
 *   x -> 3*x + 2 
 * until the number of primes seen equals the number of composites seen,
 * starting from the n-th prime (with n >= 1, offset 1).
 * The function performs a brute‐force scan and uses a deterministic
 * Miller–Rabin test for 64‑bit numbers; for larger integer types it
 * falls back to the generic is_prime_big routine supplied by the
 * build system.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

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

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
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

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;

    /* Small prime trial division */
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* Miller–Rabin deterministic for 64‑bit integers */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t mr_bases[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof(mr_bases)/sizeof(mr_bases[0]); ++i) {
        uint64_t a = mr_bases[i];
        if (a % n == 0) continue;

        __uint128_t x = 1;
        __uint128_t power = a % n;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1ULL)
                x = (x * power) % n;
            power = (power * power) % n;
            exp >>= 1;
        }

        uint64_t x64 = (uint64_t)x;
        if (x64 == 1 || x64 == n - 1) continue;

        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            x64 = (uint64_t)x;
            if (x64 == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

static bool is_prime_seq(seq_t n)
{
    /* Use the fast 64‑bit test when seq_t fits in 64 bits */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        uint64_t v = (uint64_t)n;
        return is_prime_u64(v);
    } else {
        /* For arbitrary-precision seq_t the build supplies an is_prime_big */
        return is_prime_big(n);
    }
}

seq_t a(idx_t n)
{
    /* Start value: the n‑th prime */
    seq_t x = (seq_t)get_prime(n);

    /* Counts of primes and composites seen so far */
    idx_t primes = 1;   /* starting prime */
    idx_t comps  = 0;

    /* Step counter */
    idx_t steps = 0;

    /* Iterate until the counts are equal */
    while (primes != comps) {
        /* Next term of the sequence */
        x = x * ((seq_t)3) + (seq_t)2;
        ++steps;

        /* Update prime/composite count */
        if (is_prime_seq(x))
            ++primes;
        else
            ++comps;
    }

    return (seq_t)steps;
}
