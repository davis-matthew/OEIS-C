/*
 * A293696  Irregular triangle read by rows in which row n lists the partial
 *          sums of prime numbers between 2^n+1 and 2^(n+1).
 * https://oeis.org/A293696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 224, u32 n <= 6541, u64 n <= 6541,
 *            u128 n <= 6541, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A293696 – Irregular triangle read by rows.
 * The flattened triangle is formed from the partial sums of
 * primes in the interval [ 2^r+1 , 2^(r+1) ] for r = 0,1,2,...
 * a(n) is the n‑th element of this flattening, with a(0)=2.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* ---------- Prime sieve --------------------------------------- */
static uint32_t *prime_list   = NULL;   /* array of primes up to prime_limit   */
static size_t    prime_count  = 0;      /* number of primes currently stored  */
static uint64_t  prime_limit  = 0;      /* the maximum value that has been sieved */

static void sieve_primes(uint64_t limit)
{
    /* Allocate array for sieve */
    bool *is_prime = (bool *)malloc(limit + 1);
    if (!is_prime)
        return;                        /* out of memory – abort silently */

    memset(is_prime, 1, limit + 1);
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (uint64_t p = 2; p * p <= limit; ++p)
        if (is_prime[p]) {
            for (uint64_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;
        }

    /* Count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    /* Build list */
    uint32_t *new_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_list) {
        free(is_prime);
        return;
    }
    size_t k = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_list[k++] = (uint32_t)i;

    free(is_prime);

    /* Replace old list */
    free(prime_list);
    prime_list  = new_list;
    prime_count = cnt;
    prime_limit = limit;
}

static void init_primes(void)
{
    if (prime_limit > 0)
        return;                      /* already initialised */

    /* For n up to 10 000 the largest interval is 2^18≈260 k,
       but we sieve a bit more to avoid frequent re‑sieving. */
    sieve_primes(1ULL << 22);        /* 4 194 304  – plenty */
}

/* ---------- Flat‑triangle storage ----------------------------- */
static seq_t *terms      = NULL;     /* flattened triangle values */
static size_t terms_size = 0;        /* number of terms stored    */
static size_t terms_cap  = 0;        /* allocated capacity        */

static void grow_terms(size_t needed)
{
    if (needed <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms)
        return;                      /* out of memory – abort silently */
    terms = new_terms;
    terms_cap = new_cap;
}

/* ---------- Sequence routine --------------------------------- */
static size_t next_row    = 0;        /* row to generate next           */
static size_t prime_start = 0;        /* index of first prime >= low    */

seq_t a(idx_t n)
{
    /* Ensure one‑time initialisation */
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        init_primes();
        grow_terms(8);                /* initial capacity */
    }

    /* Generate rows until we have enough terms */
    size_t target = (size_t)n;          /* safe cast for loop bounds */
    while (terms_size <= target) {

        /* Determine interval limits */
        uint64_t low  = ((uint64_t)1 << next_row) + 1;
        uint64_t high = ((uint64_t)1 << (next_row + 1));

        /* Extend sieve if necessary */
        if (high > prime_limit) {
            uint64_t new_limit = high * 2 + 1;   /* a little extra  */
            if (new_limit < high)               /* overflow guard  */
                new_limit = high + 1000;
            sieve_primes(new_limit);
        }

        /* Skip primes that are too small for this row */
        while (prime_start < prime_count &&
               prime_list[prime_start] < low)
            ++prime_start;

        /* Generate partial sums in this row */
        seq_t sum = 0;
        size_t p = prime_start;
        while (p < prime_count && prime_list[p] <= high) {
            sum += (seq_t)prime_list[p];
            grow_terms(terms_size + 1);
            terms[terms_size++] = sum;
            ++p;
        }

        /* Advance to next row */
        prime_start = p;          /* first prime > high */
        ++next_row;
    }

    return terms[target];
}
