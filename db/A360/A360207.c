/*
 * A360207  Triangular array T(n,k) read by antidiagonals: T(2,1) = 1;
 *          otherwise T(n,k) = p(n)!/(p(k)!*p(n-k)!), where p(0)=1 and
 *          p(m)=prime(m) for m > 0.
 * https://oeis.org/A360207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2. T(r,k) = 1 when (r==2 && k==1); otherwise T(r,k) =
 *            p(r)!/(p(k)! * p(r-k)!) where p(0)=1 and p(m)=prime(m) for m>0.
 *            The factorials are evaluated via prime exponents using
 *            Legendre's formula
 * Range:     u8 n <= 15, u16 n <= 22, u32 n <= 39, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* ---------- Local prime table ------------------------------------- */

static uint32_t local_primes[2000];
static size_t local_prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    const int LIMIT = 2000;
    bool sieve[LIMIT + 1];
    for (int i = 0; i <= LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= LIMIT; j += i) sieve[j] = false;
        }
    }
    for (int i = 2; i <= LIMIT; ++i) {
        if (sieve[i]) local_primes[local_prime_cnt++] = (uint32_t)i;
    }
    primes_inited = true;
}

static inline uint32_t get_prime_idx(uint32_t idx)
{
    if (idx == 0) return 1;            /* p(0)=1 by definition   */
    return local_primes[idx - 1];      /* 1‑based index for primes  */
}

/* ------------------------------------------------------------------ */
/* ---------- Integer square root ----------------------------------- */

static inline uint64_t isqrt_u64(uint64_t v)
{
    uint64_t low = 0, high = v;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* ------------------------------------------------------------------ */
/* ---------- Legendre exponent ------------------------------------- */

static inline int legendre_exp(uint32_t n, uint32_t p)
{
    int e = 0;
    while (n) {
        n /= p;
        e += (int)n;
    }
    return e;
}

/* ------------------------------------------------------------------ */
/* ---------- Utils helpers ------------------------------------------ */
/* The build provides these functions; we include the most useful ones. */

#include "../utils/seq_mul.h"      /* seq_mul(seq_t a, seq_t b)  */
#include "../utils/pow_uint.h"     /* pow_uint(uint64_t base, idx_t exp)  */

/* ------------------------------------------------------------------ */
/* ---------- Main sequence function --------------------------------- */

seq_t a(idx_t n)
{
    /* Initialise prime table once */
    init_primes();

    /* Find (r,k) from the antidiagonal index n */
    uint64_t t = 8ULL * n + 1ULL;
    uint64_t s = isqrt_u64(t);          /* floor(sqrt(8*n+1))   */
    uint64_t r = (s - 1ULL) / 2ULL;     /* row index            */
    uint64_t row_start = r * (r + 1) / 2;
    uint64_t k = n - row_start;         /* column index         */

    /* Special override */
    if (r == 2ULL && k == 1ULL) return (seq_t)1;

    /* Get the prime values p(r), p(k), p(r-k) */
    uint32_t p_r = (uint32_t)get_prime_idx((uint32_t)r);
    uint32_t p_k = (uint32_t)get_prime_idx((uint32_t)k);
    uint32_t p_rk = (uint32_t)get_prime_idx((uint32_t)(r - k));

    /* Compute the factorial ratio via prime exponents */
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < local_prime_cnt; ++i) {
        uint32_t p = local_primes[i];
        if (p > p_r) break;
        int exp = legendre_exp(p_r, p) - legendre_exp(p_k, p) - legendre_exp(p_rk, p);
        if (exp > 0) {
            seq_t term = pow_uint(p, (idx_t)exp);
            result = seq_mul(result, term);
        }
    }

    return result;
}
