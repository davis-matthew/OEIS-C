/*
 * A279372  Expansion of (Sum_{k>=1} x^(prime(k)^2))^2.
 * https://oeis.org/A279372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs of primes (p,q) with p^2 + q^2 =
 *            n
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple sieve to generate all primes up to
   sqrt(max_requested_n) and then for each prime p checks whether
   n - p^2 is a square of a prime.  The sieve data structures are
   kept static so that repeated calls with arbitrary n are O(n) at worst,
   and fast for n ≤ 10000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t _isqrt(idx_t n) {
    /* integer floor sqrt, works for all idx_t values small enough
       that mid*mid does not overflow (our use is n ≤ 10^4). */
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high; /* floor(sqrt(n)) */
}

/* Static sieve state */
static idx_t prime_limit = 0;
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static bool *prime_flag = NULL; /* prime_flag[i] = true iff i is prime */

static void _ensure_primes_up_to(idx_t limit) {
    if (limit <= prime_limit)
        return;
    /* Allocate flag array up to limit */
    bool *new_flag = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_flag)
        return; /* allocation failed; leave state unchanged */
    for (idx_t i = 0; i <= limit; ++i)
        new_flag[i] = true;
    if (limit >= 0) new_flag[0] = false;
    if (limit >= 1) new_flag[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (new_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_flag[j] = false;
        }
    }
    /* Build prime list */
    size_t new_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (new_flag[i])
            ++new_cnt;
    uint64_t *new_list = (uint64_t *)malloc(new_cnt * sizeof(uint64_t));
    if (!new_list) {
        free(new_flag);
        return; /* allocation failed; leave state unchanged */
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (new_flag[i])
            new_list[idx++] = (uint64_t)i;
    /* Replace old arrays */
    free(prime_list);
    free(prime_flag);
    prime_list = new_list;
    prime_flag = new_flag;
    prime_cnt = new_cnt;
    prime_limit = limit;
}

seq_t a(idx_t n) {
    if (n < 4) /* smallest prime square is 2^2 = 4 */
        return (seq_t)0;
    /* Ensure primes up to sqrt(n) are available */
    idx_t lim = _isqrt(n);
    _ensure_primes_up_to(lim);
    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        idx_t sq = p * p;
        if (sq > n)
            break;
        idx_t rem = n - sq;
        idx_t r = _isqrt(rem);
        if (r * r != rem)
            continue;      /* not a perfect square */
        if (r < 2)
            continue;      /* 1 is not prime */
        /* r is guaranteed to be ≤ lim, so prime_flag covers it */
        if (prime_flag[r]) {
            ++count;       /* ordered pair (p, sqrt(rem)) */
        }
    }
    return count;
}
