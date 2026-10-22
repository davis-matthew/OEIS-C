/*
 * A356833  Primes p such that the minimum number of divisors among the
 *          numbers between p and NextPrime(p) is a square.
 * https://oeis.org/A356833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Sequence: primes p such that the minimum number of divisors
   τ(k) for p < k < next_prime(p) is a perfect square.
   This implementation builds primes on demand, computes τ(k)
   by trial division using the prime list, and caches the found
   primes in the sequence order.  All operations use only the
   allowed integer operators; no floating point or math library
   usage occurs.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer square root for uint64_t (used in divisor counting) */
static uint64_t uint64_sqrt(uint64_t x)
{
    uint64_t lo = 0, hi = (1ULL << 32) - 1;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* --------------------------------------------------------------------- */
/* Prime list (sufficient for primes needed by the sequence)            */
/* --------------------------------------------------------------------- */
static uint64_t *prime_list   = NULL;
static size_t   prime_cap    = 0;
static size_t   prime_cnt    = 0;

static void ensure_prime_capacity(size_t need)
{
    if (prime_cap < need) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        if (new_cap < need)
            new_cap = need;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

static void init_primes(void)
{
    if (prime_cnt == 0) {
        ensure_prime_capacity(16);
        prime_list[0] = 2;
        prime_list[1] = 3;
        prime_cnt = 2;
    }
}

/* Find the smallest prime strictly greater than cur;
   append it to prime_list if it is new. */
static uint64_t next_prime_after(uint64_t cur)
{
    uint64_t cand = cur + 1;
    if (cand <= 2) cand = 2;
    if (cand > 2 && (cand & 1ULL) == 0) cand++;
    for (;;) {
        uint64_t limit = uint64_sqrt(cand);
        bool prime = true;
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
            if (cand % prime_list[i] == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            if (cand > prime_list[prime_cnt - 1]) {
                ensure_prime_capacity(prime_cnt + 1);
                prime_list[prime_cnt++] = cand;
            }
            return cand;
        }
        cand += 2;
    }
}

/* --------------------------------------------------------------------- */
/* Divisor count τ(n) for 64‑bit integer n                              */
/* --------------------------------------------------------------------- */
static uint64_t tau_of(uint64_t n)
{
    if (n < 2) return 1;
    uint64_t res = 1;
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= m; ++i) {
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            res *= (exp + 1);
        }
    }
    if (m > 1) res *= 2;
    return res;
}

/* --------------------------------------------------------------------- */
/* Answer list (the sequence values)                                     */
/* --------------------------------------------------------------------- */
static seq_t *ans_list  = NULL;
static size_t ans_cap   = 0;
static size_t ans_cnt   = 0;
static size_t next_prime_idx = 0;   /* index of next prime to process */

static void ensure_ans_capacity(size_t need)
{
    if (ans_cap < need) {
        size_t new_cap = ans_cap ? ans_cap * 2 : 16;
        if (new_cap < need)
            new_cap = need;
        seq_t *tmp = (seq_t *)realloc(ans_list, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        ans_list = tmp;
        ans_cap  = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function: a(idx_t n)                                    */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, but return 0 */
    init_primes();

    /* produce sequence values up to the requested index n */
    while (ans_cnt < (size_t)n) {
        /* ensure that prime_list has the next prime after prime_list[next_prime_idx] */
        if (next_prime_idx + 1 >= prime_cnt) {
            next_prime_after(prime_list[next_prime_idx]);
        }

        uint64_t p = prime_list[next_prime_idx];
        uint64_t q = prime_list[next_prime_idx + 1];

        /* find the minimal τ(k) in the interval (p, q) */
        uint64_t min_tau = UINT64_MAX;
        for (uint64_t k = p + 1; k < q; ++k) {
            uint64_t t = tau_of(k);
            if (t < min_tau)
                min_tau = t;
        }

        /* test whether min_tau is a perfect square */
        uint64_t root = uint64_sqrt(min_tau);
        if (root * root == min_tau) {
            ensure_ans_capacity(ans_cnt + 1);
            ans_list[ans_cnt++] = (seq_t)p;  /* cast a uint64_t to seq_t */
        }

        next_prime_idx++;
    }

    return ans_list[n - 1];
}
