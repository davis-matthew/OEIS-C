/*
 * A300562  Condensed deep factorization of n, in binary. (Remove all trailing
 *          0's and one trailing 1 from A300560.)
 * https://oeis.org/A300562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007088(A300563(n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   build_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the decimal integer whose decimal digits are the binary
 * representation of the condensed deep factorization of n (after
 * removing all trailing zeros and the final trailing 1). */
 
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes.h"     /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */
#include "../utils/factor_u64.h"      /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
 
static idx_t *primes = NULL;       /* array of primes (idx_t = uint64_t) */
static size_t primes_cnt = 0;      /* number of primes in the array */
static idx_t primes_limit = 0;     /* maximum n for which primes have been generated */
 
/* Ensure that the primes array contains all primes ≤ max_n. */
static void ensure_primes(idx_t max_n)
{
    if (max_n <= primes_limit) return;
    free(primes);
    build_primes(max_n, &primes, &primes_cnt);
    primes_limit = max_n;
}
 
/* Binary search to find the 1‑based index of a prime p in the primes array. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p) return mid + 1;          /* 1‑based index */
        if (primes[mid] < p)   lo = mid + 1;
        else                   hi = mid;
    }
    return 0; /* should never happen for a valid input */
}
 
/* Recursively encode a positive integer m into the global bit buffer 'buf'.
 * Bits are appended to buf in order from most significant to least
 * significant. The buffer must have enough space for all bits produced. */
static void encode_uint(uint64_t m, uint8_t *buf, size_t *len)
{
    if (m == 1) return;                     /* empty representation */
 
    uint64_t primes_local[64];
    uint64_t exp_local[64];
    size_t count = 0;
    factor_u64(m, primes_local, exp_local, &count);
 
    for (size_t i = 0; i < count; ++i) {
        uint64_t p  = primes_local[i];
        uint64_t e  = exp_local[i];
        size_t idx  = prime_index(p);        /* 1‑based index of prime p */
 
        /* Outer '[' */
        buf[(*len)++] = 1;
        /* Representation of the prime index */
        encode_uint((uint64_t)idx, buf, len);
        /* Inner '[' for the exponent */
        buf[(*len)++] = 1;
        /* Representation of the exponent */
        encode_uint(e, buf, len);
        /* Inner ']' */
        buf[(*len)++] = 0;
        /* Outer ']' */
        buf[(*len)++] = 0;
    }
}
 
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
 
    /* Cache primes up to n. */
    ensure_primes(n);
 
    /* Buffer for bits.  8192 bytes is ample for the targeted range. */
    static uint8_t bits[8192];
    size_t len = 0;
 
    encode_uint((uint64_t)n, bits, &len);
 
    /* Remove trailing zeros. */
    while (len > 0 && bits[len - 1] == 0) --len;
    /* Remove the final trailing 1. */
    if (len > 0 && bits[len - 1] == 1) --len;
 
    /* Convert the remaining bits to a decimal integer whose digits are
     * the bits of the binary string. */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * 10 + bits[i];
    }
    return result;
}
