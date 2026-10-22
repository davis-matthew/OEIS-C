/*
 * A381958  Numerator of the sum of the reciprocals of the indices of distinct
 *          prime factors of n.
 * https://oeis.org/A381958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the sum of reciprocals of the indices of
 *            distinct prime factors of n. For prime p_j, pi(p_j) is its
 *            1‑based index in the ordered list of primes
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ---------- prime list utilities ---------- */

static uint32_t *prime_list = NULL;   /* array of primes up to prime_limit */
static size_t prime_count  = 0;       /* number of primes in the list      */
static uint64_t prime_limit = 0;      /* largest prime currently in list    */

/* Ensure that prime_list contains all primes ≤ limit.  Uses a simple
   sieve of Eratosthenes.  The array is allocated and all previous
   storage is freed.  The function is thread‑safe only if used in a
   single thread. */
static void ensure_primes_upto(uint64_t limit)
{
    if (prime_limit >= limit)
        return;

    /* Allocate flags for composites */
    size_t sz = (size_t)(limit + 1);
    uint8_t *is_composite = (uint8_t *)calloc(sz, sizeof(uint8_t));
    if (!is_composite)
        return;  /* allocation failure – we simply fail; not expected */

    size_t i, j;
    for (i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (j = i * i; j <= limit; j += i)
                is_composite[j] = 1;
        }
    }

    /* Count primes */
    size_t count = 0;
    for (i = 2; i <= limit; ++i)
        if (!is_composite[i])
            ++count;

    /* Allocate and fill prime list */
    uint32_t *new_list = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!new_list) {
        free(is_composite);
        return;
    }
    size_t idx = 0;
    for (i = 2; i <= limit; ++i)
        if (!is_composite[i])
            new_list[idx++] = (uint32_t)i;

    free(is_composite);
    free(prime_list);
    prime_list  = new_list;
    prime_count = count;
    prime_limit = limit;
}

/* ---------- gcd for signed __int128 ---------- */

static __int128 gcd128(__int128 a, __int128 b)
{
    while (b != 0) {
        __int128 r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ---------- sequence value ---------- */

seq_t a(idx_t n)
{
    if (n <= 1)                    /* a(1) = 0  */
        return (seq_t)0;

    uint64_t rem = (uint64_t)n;     /* number to factor */
    __int128 num = 0;               /* current numerator of sum   */
    __int128 den = 1;               /* current denominator of sum */

    /* Ensure prime list covers n; this guarantees we have all needed
       primes for factorisation. */
    if (rem > prime_limit)
        ensure_primes_upto(rem);

    while (rem > 1) {
        uint64_t p = 0;     /* smallest prime factor found */
        size_t i;

        /* Search for a prime factor among primes in the list
           (the list contains all primes up to rem). */
        for (i = 0; i < prime_count; ++i) {
            uint32_t pr = prime_list[i];
            if ((uint64_t)pr * (uint64_t)pr > rem)
                break;
            if (rem % pr == 0) {
                p = pr;
                break;
            }
        }

        if (p == 0) { /* rem itself is prime */
            p = rem;
        }

        /* Consume all powers of the found prime factor */
        while (rem % p == 0)
            rem /= p;

        /* Find the index (pi(p)) of the prime factor p */
        size_t lo = 0, hi = prime_count, pos = 0;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (prime_list[mid] == (uint32_t)p) { pos = mid; break; }
            else if (prime_list[mid] < (uint32_t)p) lo = mid + 1;
            else hi = mid;
        }
        uint64_t idx = (uint64_t)(pos + 1);  /* 1‑based index */

        /* Update fraction: num/den + 1/idx = (num*idx + den) / (den*idx) */
        __int128 new_num = num * idx + den;
        __int128 new_den = den * idx;
        __int128 g = gcd128(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* Return the numerator as seq_t */
    return (seq_t)num;
}
