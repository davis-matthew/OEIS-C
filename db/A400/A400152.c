/*
 * A400152  a(n) = p^{the most significant digit of n in primorial base,
 *          A049345}, where p is the corresponding radix prime; a(0) = 1.
 * https://oeis.org/A400152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p^d where d is the most significant digit of n in
 *            primorial base, p is the corresponding radix prime; a(0) = 1
 * Range:     u8 n <= 23, u16 n <= 179, u32 n <= 2099, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/big_pow.h"

/* maximum number of primes we will need – far more than any
   uint64_t primorial requires (the 53rd prime is already > 2^64). */
#define MAX_PRIMES 200

static uint64_t primes[MAX_PRIMES];          /* primes[0] = 2, ...          */
static seq_t   primorials[MAX_PRIMES + 1];   /* primorials[0] = 1, etc.     */
static size_t  prime_cnt = 0;                /* number of primes stored     */
static bool    cache_inited = false;         /* has the cache been built?   */

/* Build the prime / primorial cache up to the smallest primorial
   larger than n.  This is done on demand, so a(n) can be called
   in any order.  The loop is tiny – only a few primes are needed. */
static void ensure_cache(idx_t n)
{
    if (!cache_inited) {
        /* initialise with the first prime */
        primes[0] = 2ULL;
        prime_cnt = 1;
        primorials[0] = (seq_t)1;          /* product of 0 primes    */
        primorials[1] = (seq_t)2;          /* product of first prime */
        cache_inited = true;
    }

    /* Keep generating primes until primorial[prime_cnt] > n */
    while (primorials[prime_cnt] <= (seq_t)n) {
        uint64_t cand = primes[prime_cnt - 1] + 1;
        /* find the next prime; our range is tiny, so a trial
           division test using is_prime_small() is ample. */
        while (!is_prime_small(cand))
            ++cand;

        if (prime_cnt >= MAX_PRIMES)
            /* We should never run out of slots for such tiny n. */
            break;

        primes[prime_cnt]          = cand;
        primorials[prime_cnt + 1]  = primorials[prime_cnt] * (seq_t)cand;
        ++prime_cnt;
    }
}

/* Main sequence function.  idx_t is guaranteed to be unsigned and
   seq_t is an unsigned integer type or an arbitrary‑precision
   integer class. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* a(0) = 1 */

    ensure_cache(n);

    /* Find the largest j such that primorials[j] <= n < primorials[j+1] */
    size_t j = 0;
    while (j + 1 <= prime_cnt && primorials[j + 1] <= (seq_t)n)
        ++j;

    /* Most significant digit in primorial base is floor(n / primorial[j]) */
    seq_t msd = (seq_t)n / primorials[j];

    /* Corresponding radix prime is primes[j] */
    return big_pow(primes[j], msd);
}
