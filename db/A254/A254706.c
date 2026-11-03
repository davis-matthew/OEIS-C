/*
 * A254706  a(n) = Catalan(2*n) mod prime(n).
 * https://oeis.org/A254706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(2*n) mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 6586, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9916 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Catalan(2*n) = binom(4*n,2*n) - binom(4*n,2*n+1)  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static uint64_t binom_small(uint64_t N, uint64_t K, uint64_t mod, uint64_t *inv)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;
    if (K == 0) return 1ULL;

    inv[1] = 1ULL;
    for (uint64_t i = 2; i <= K; ++i)
        inv[i] = (mod - (mod / i) * inv[mod % i] % mod);

    uint64_t res = 1ULL;
    for (uint64_t i = 1; i <= K; ++i) {
        uint64_t num = (N - K + i) % mod;
        res = mul_mod_uint64(res, num, mod);
        res = mul_mod_uint64(res, inv[i], mod);
    }
    return res;
}

/* Lucas theorem for n,k < 2^63, prime mod < 2^63 */
static uint64_t binom_lucas(uint64_t N, uint64_t K, uint64_t mod, uint64_t *inv)
{
    if (K == 0) return 1ULL;
    uint64_t ni = N % mod;
    uint64_t ki = K % mod;
    if (ki > ni) return 0ULL;
    uint64_t sub = binom_lucas(N / mod, K / mod, mod, inv);
    uint64_t small = binom_small(ni, ki, mod, inv);
    return mul_mod_uint64(sub, small, mod);
}

seq_t a(idx_t n)
{
    /* prime at position n (1‑based) */
    seq_t p_seq = (seq_t)get_prime(n);
    uint64_t p = (uint64_t)p_seq;    /* p fits in 64 bits for n <= 10000 */

    uint64_t N  = 4ULL * (uint64_t)n;
    uint64_t K1 = 2ULL * (uint64_t)n;
    uint64_t K2 = 2ULL * (uint64_t)n + 1ULL;

    /* buffer for modular inverses; max K <= 2*n <= 20000 for n<=10000 */
    static uint64_t inv_buf[20001];

    uint64_t bin1 = binom_lucas(N, K1, p, inv_buf);
    uint64_t bin2 = binom_lucas(N, K2, p, inv_buf);

    uint64_t result = (bin1 + p - bin2) % p;
    return (seq_t)result;
}
