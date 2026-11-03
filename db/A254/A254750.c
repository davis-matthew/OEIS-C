/*
 * A254750  Numbers such that, in base 10, all their proper prefixes and
 *          suffixes represent composites.
 * https://oeis.org/A254750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 3774, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9848 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A254750 – numbers whose every proper prefix and suffix is composite.
   Computed by brute‑force search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
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

/* extract decimal digits of n (most significant first) */
static void digits_of_seq(seq_t n, uint8_t buf[64], size_t *len)
{
    size_t i = 0;
    seq_t tmp = n;
    if (tmp == 0) {
        buf[0] = 0;
        *len = 1;
        return;
    }
    while (tmp != 0 && i < 64) {
        buf[i++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    /* reverse to most significant first */
    *len = i;
    for (size_t l = 0, r = i - 1; l < r; ++l, --r) {
        uint8_t b = buf[l];
        buf[l] = buf[r];
        buf[r] = b;
    }
}

/* --- Helpers ----------------------------------------------------- */
static const size_t LIMIT = 4000000;            /* sieve limit */

/* isPrime[i] == true  -> i is prime  (i >= 2)
 * isPrime[i] == false -> i is composite */
static bool *isPrime = NULL;

/* cache of the sequence */
static seq_t *cache = NULL;
static size_t cacheLen = 0;   /* number of terms stored */
static size_t cacheCap = 0;

/* sieve of Eratosthenes up to LIMIT (build once) */
static void init_sieve(void)
{
    if (isPrime != NULL)
        return;  /* already initialized */

    isPrime = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (isPrime == NULL)
        exit(1);  /* memory allocation failed */

    /* Assume all numbers >=2 are prime at start */
    for (size_t i = 2; i <= LIMIT; ++i)
        isPrime[i] = true;

    /* integer sqrt of LIMIT */
    size_t sqrtLimit = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i)
        sqrtLimit = i;

    /* Sieve */
    for (size_t i = 2; i <= sqrtLimit; ++i) {
        if (isPrime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                isPrime[j] = false;
        }
    }
}

/* test whether x is composite (x>1) */
static bool is_composite(seq_t x)
{
    if (x <= (seq_t)1)
        return false;
    if (x <= (seq_t)LIMIT)
        return !isPrime[(size_t)x];
    return !is_prime_big(x);
}

/* test if a number satisfies the sequence property */
static bool valid_candidate(seq_t x)
{
    uint8_t digs[64];
    size_t len;
    digits_of_seq(x, digs, &len);

    if (len < 2)   /* proper prefixes and suffixes must exist */
        return false;

    /* prefixes */
    seq_t pref = 0;
    for (size_t i = 0; i < len - 1; ++i) {
        pref = pref * 10 + digs[i];
        if (!is_composite(pref))
            return false;
    }

    /* suffixes */
    for (size_t split = 1; split < len; ++split) {
        seq_t suff = 0;
        for (size_t j = split; j < len; ++j)
            suff = suff * 10 + digs[j];
        if (!is_composite(suff))
            return false;
    }
    return true;
}

/* Sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;     /* out-of-range, return 0 */

    /* initialise on first call */
    init_sieve();

    /* lazy initialization of cache start value */
    static seq_t nextCandidate = (seq_t)44;

    /* grow cache if needed */
    while (cacheLen < (size_t)n) {
        /* find next valid candidate */
        while (!valid_candidate(nextCandidate))
            ++nextCandidate;

        /* store it */
        if (cacheLen == cacheCap) {
            cacheCap = (cacheCap == 0) ? 128 : cacheCap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, cacheCap);
            if (tmp == NULL)
                exit(1);      /* memory allocation failed */
            cache = tmp;
        }
        cache[cacheLen++] = nextCandidate;
        ++nextCandidate;
    }

    return cache[n - 1];
}
