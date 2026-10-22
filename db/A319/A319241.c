/*
 * A319241  Heinz numbers of strict integer partitions of even numbers.
 *          Squarefree numbers whose prime indices sum to an even number.
 * https://oeis.org/A319241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th squarefree integer whose sum of prime indices
 *            (in the prime sequence 2,3,5,7,…) is even. The function
 *            generates candidates in ascending order, tests squarefree‑ness
 *            and parity, and stores the terms so that subsequent calls are
 *            fast
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>   /* for sqrt */

/* --- cache of computed sequence values --------------------------------- */
static seq_t *cache = NULL;          /* dynamic array of terms          */
static size_t cache_cap  = 0;        /* allocated capacity              */
static size_t cache_size = 0;        /* number of terms stored          */

/* Ensure capacity of the cache array. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    while (new_cap < needed) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Append a new element to the cache. */
static void cache_push(seq_t v)
{
    if (cache_size == cache_cap) {
        ensure_cache_cap(cache_size + 1);
    }
    cache[cache_size++] = v;
}

/* --- prime table ------------------------------------------------------- */
static uint64_t *primes = NULL;      /* list of primes                   */
static size_t prime_cap  = 0;        /* allocated capacity               */
static size_t prime_cnt  = 0;        /* number of primes stored          */
static uint64_t prime_last_checked = 1;  /* last integer checked for primality */

/* Integer floor of sqrt, for uint64_t values. */
static uint64_t isqrt64(uint64_t x)
{
    uint64_t r = (uint64_t)sqrt((double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* Expand the prime list up to the given limit.  */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= prime_last_checked) return;
    for (uint64_t num = prime_last_checked + 1; num <= limit; ++num) {
        bool is_p = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > num) break;
            if (num % p == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (prime_cnt == prime_cap) {
                prime_cap = (prime_cap == 0) ? 8 : prime_cap * 2;
                primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
            }
            primes[prime_cnt++] = num;
        }
    }
    prime_last_checked = limit;
}

/* Return the 1‑based index of a prime p in the primes array. */
static uint64_t prime_index(uint64_t p)
{
    size_t left = 0, right = prime_cnt;
    while (left < right) {
        size_t mid = (left + right) >> 1;
        uint64_t q = primes[mid];
        if (q == p) return (uint64_t)(mid + 1);  /* 1‑based index */
        if (q < p) left = mid + 1;
        else      right = mid;
    }
    /* Should never be reached for prime p. */
    return 0;
}

/* --- Cache generating routine ----------------------------------------- */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_size >= n) return;

    static uint64_t last_tested = 2;     /* next integer to test (>=2)   */
    static bool first_term_added = false;

    if (!first_term_added) {            /* add the initial term 1 */
        cache_push((seq_t)1);
        first_term_added = true;
    }

    while ((idx_t)cache_size < n) {
        uint64_t cand = last_tested++;

        /* Factor cand and test the conditions. */
        uint64_t temp = cand;
        bool squarefree = true;
        bool parity = false;   /* false -> even sum of indices */

        /* Ensure we have primes up to sqrt(temp). */
        uint64_t lim = isqrt64(temp);
        ensure_primes_up_to(lim);

        for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
            uint64_t p = primes[i];
            if (temp % p == 0) {
                int exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                    if (exp > 1) { squarefree = false; break; }
                }
                if (!squarefree) break;
                uint64_t idx = prime_index(p);   /* 1‑based index */
                parity ^= (idx & 1ULL);          /* toggle if odd */
            }
        }
        if (!squarefree) continue;

        if (temp > 1) {                       /* remaining prime factor */
            ensure_primes_up_to(temp);
            uint64_t idx = prime_index(temp);
            parity ^= (idx & 1ULL);
        }

        if (!parity) cache_push((seq_t)cand); /* even sum → store */
    }
}

/* ------------------------------------------------------------------------ */
/* Main sequence function                                                   */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;  /* undefined for n < 1, safe fallback */
    ensure_cache(n);
    return cache[n - 1];
}
