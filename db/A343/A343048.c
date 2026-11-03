/*
 * A343048  a(n) is the least number whose sum of digits in primorial base
 *          equals n.
 * https://oeis.org/A343048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 36, u32 n <= 109, u64 n <= 342,
 *            u128 n <= 1135, big any n
 * Validated: standard, pass, 60 terms through n = 9184 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* Computed by greedy “low‑weight” algorithm.  
   For a given digit sum n, it repeatedly takes as many digits as
   possible from the current primorial base position (whose weight is
   the product of all preceding primes).  This yields the smallest
   number whose primorial‑base digit sum equals n.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Prime generation keeps a static list of primes.  */
static void _ensure_next_prime(void)
{
    static uint64_t last = 1;                /* last prime in the list   */
    static uint64_t candidate = 3;           /* next candidate to test  */
    static bool initialized = false;

    /* First time initialise the list with the prime 2.  */
    if (!initialized) {
        last = 2;
        initialized = true;
    }

    for (;;) {
        bool prime = true;
        for (uint64_t p = 2; p * p <= candidate; ++p) {
            if (candidate % p == 0) { prime = false; break; }
        }
        if (prime) {
            last = candidate;
            /* Add the prime to a dynamic array?  For simplicity we
               keep a linked list in a static buffer of size 2000.  */
            static uint64_t primes[2000];
            static size_t prime_cnt = 0;
            if (prime_cnt < 2000)
                primes[prime_cnt++] = last;
            else
                ; /* Should never happen for n <= 10000.   */
            candidate += 2; /* next odd candidate */
            return;
        }
        candidate += 2;
    }
}

/* The list of primes as a static array.  We'll rebuild it lazily
   using the helper above.  */
static struct {
    uint64_t *data;
    size_t   size;
    size_t   capacity;
} _prime_list = {NULL, 0, 0};

static void _add_prime(uint64_t p)
{
    if (_prime_list.size == _prime_list.capacity) {
        size_t newcap = (_prime_list.capacity == 0) ? 16 : _prime_list.capacity * 2;
        uint64_t *newarr = (uint64_t *)realloc(_prime_list.data, newcap * sizeof(uint64_t));
        if (!newarr) return; /* malloc failure – should never happen */
        _prime_list.data   = newarr;
        _prime_list.capacity = newcap;
    }
    _prime_list.data[_prime_list.size++] = p;
}

/* Fill primes up to count needed.  */
static void _ensure_primes_needed(size_t needed)
{
    static bool primes_initialized = false;
    if (!primes_initialized) {
        _prime_list.data = NULL;
        _prime_list.size = 0;
        _prime_list.capacity = 0;
        /* First prime is 2.  */
        _add_prime(2);
        primes_initialized = true;
    }
    while (_prime_list.size < needed) {
        /* Generate next prime.  */
        static uint64_t last = 2;
        uint64_t p = last + 1;
        if (p <= 2) p = 3;
        if (p % 2 == 0) p++;
        for (;;) {
            bool ok = true;
            for (size_t i = 0; i < _prime_list.size; ++i) {
                uint64_t pd = _prime_list.data[i];
                if (pd * pd > p) break;
                if (p % pd == 0) { ok = false; break; }
            }
            if (ok) break;
            p += 2;
        }
        last = p;
        _add_prime(p);
    }
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* The following type casts are required by the build.  */
    seq_t rem = (seq_t)n;     /* remaining digit sum to allocate   */
    seq_t m   = 0;            /* resulting minimal number          */
    seq_t weight = 1;         /* weight of the current position   */
    size_t pos = 0;           /* index into the prime list         */

    while (rem > 0) {
        /* Ensure we have at least (pos+1) primes.  */
        _ensure_primes_needed(pos + 1);
        uint64_t p = _prime_list.data[pos];
        seq_t bound = (seq_t)(p - 1);
        seq_t use = rem < bound ? rem : bound;
        m += use * weight;
        rem -= use;
        weight *= (seq_t)p;
        ++pos;
    }
    return m;
}
