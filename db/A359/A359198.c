/*
 * A359198  Numbers k such that 2*phi(k)-k is a prime, where phi is A000010.
 * https://oeis.org/A359198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 32, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 2*phi(k)-k is a prime.  One retrieves the nth odd integer
   that satisfies the condition (OEIS A359198).  This implementation
   generates primes up to a fixed limit, computes φ(k) by factorisation,
   tests the resulting value for primality using the library function
   is_prime_big, and memoises found terms. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
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

static const idx_t PRIME_LIMIT = 1000000ULL;   /* enough for n up to ~10 000 */

static uint32_t *primes   = NULL;   /* primes up to PRIME_LIMIT */
static size_t    primeCnt  = 0;
static seq_t    *good      = NULL;   /* list of good k's */
static size_t    goodCnt   = 0;
static size_t    goodCap   = 0;
static idx_t     nextCandidate = 3;   /* next odd k to test */

/* ------------------------------------------------------------------ */
/* Simple sieve to generate primes up to PRIME_LIMIT                 */
static void init_primes(void)
{
    if (primes != NULL) return;   /* already initialised */

    /* estimate number of primes (upper bound) */
    primeCnt = 0;
    size_t approxCnt = PRIME_LIMIT / 10 + 1;   /* < π(1 000 000) = 78498 */
    primes = (uint32_t*)malloc(approxCnt * sizeof(uint32_t));

    /* mark composites */
    bool *isComp = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));

    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!isComp[i]) {
            if (primeCnt == approxCnt)        /* reallocate if needed */
                primes = (uint32_t*)realloc(primes, (approxCnt *= 2) * sizeof(uint32_t));
            primes[primeCnt++] = (uint32_t)i;
            if (i * i <= PRIME_LIMIT) {
                for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                    isComp[j] = true;
            }
        }
    }
    free(isComp);
}

/* ------------------------------------------------------------------ */
/* Compute φ(n) via prime factorisation using the pre‑computed primes */
static uint64_t phi_of(idx_t n)
{
    idx_t temp = n;
    uint64_t result = (uint64_t)n;
    for (size_t i = 0; i < primeCnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > temp) break;
        if (temp % p == 0) {
            result = result / p * (p - 1);
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) result = result / temp * (temp - 1);
    return result;
}

/* ------------------------------------------------------------------ */
/* Ensure that good[] contains at least n terms */
static void extend_to(idx_t n)
{
    if (n <= goodCnt) return;

    while (goodCnt < n) {
        /* skip even numbers */
        if ((nextCandidate & 1) == 0) ++nextCandidate;

        uint64_t phi_k = phi_of(nextCandidate);
        uint64_t val = 2 * phi_k - (uint64_t)nextCandidate;

        if (val > 1 && is_prime_big((seq_t)val)) {
            if (goodCnt == goodCap) {          /* grow the array */
                goodCap = goodCap ? goodCap * 2 : 64;
                good = mem_resize(seq_t, good, goodCap);
            }
            good[goodCnt++] = (seq_t)nextCandidate;
        }
        nextCandidate += 2;   /* stay odd */
    }
}

/* ------------------------------------------------------------------ */
/* External interface as required by the build system                 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */
    init_primes();
    extend_to(n);
    return good[n - 1];
}
