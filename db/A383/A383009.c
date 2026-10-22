/*
 * A383009  Indices of the even terms in the sequence of cubefree numbers.
 * https://oeis.org/A383009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = index of the nth even cubefree number in the
   sequence of cubefree numbers.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* === prime generation helper ==================================== */
static uint32_t primes_arr[200000] = {2};
static size_t   prime_cnt   = 1;
static uint32_t primes_limit = 2;

/* extend the prime table up to 'limit' (inclusive) */
static void ensure_primes_up_to(uint32_t limit)
{
    if (limit <= primes_limit) return;
    for (uint32_t cand = primes_limit + 1; cand <= limit; ++cand) {
        bool isprime = true;
        uint32_t sq = (uint32_t)sqrt((double)cand);
        for (size_t i = 0; i < prime_cnt && primes_arr[i] <= sq; ++i) {
            if (cand % primes_arr[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            if (prime_cnt >= sizeof(primes_arr)/sizeof(primes_arr[0]))
                return;                      /* should not happen for small limits */
            primes_arr[prime_cnt++] = cand;
        }
    }
    primes_limit = limit;
}

/* === cubefree test ============================================= */
static bool is_cubefree(uint64_t x)
{
    uint64_t tmp = x;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t  p  = primes_arr[i];
        unsigned __int128 p3 = (unsigned __int128)p * p * p;
        if (p3 > tmp) break;
        if (tmp % p == 0) {
            unsigned cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
                if (cnt > 2) return false;          /* exponent ≥ 3 */
            }
        }
    }
    /* remaining factor(s) cannot have exponent > 2 */
    return true;
}

/* === main sequence function ===================================== */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                            /* offset is 1 */

    seq_t  even_cnt = 0;  /* how many even cubefree numbers seen */
    seq_t  idx      = 0;  /* current index in the cubefree sequence */
    uint64_t m = 1;       /* candidate number */

    while (true) {
        uint32_t limit = (uint32_t)sqrt((double)m) + 1;
        ensure_primes_up_to(limit);

        if (is_cubefree(m)) {
            ++idx;
            if ((m & 1ULL) == 0) {
                ++even_cnt;
                if (even_cnt == n) return idx;
            }
        }
        ++m;
    }
}
