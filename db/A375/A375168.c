/*
 * A375168  Numbers k with omega(k) = A001221(k) > 2 such that the set of
 *          distinct primes dividing k arranged in ascending order is an
 *          arithmetic progression.
 * https://oeis.org/A375168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A375168
 *  Numbers k with ω(k)>2 such that the set of distinct prime divisors of k
 *  (arranged in ascending order) is an arithmetic progression.
 *  a(n) returns the nth term (offset 1).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Typedefs supplied by the build environment */
typedef uint64_t idx_t;   /* index type (unsigned) */

/* ----------------------------------------------------------- */
/*  Dynamic array of seq_t values (the sequence itself).       */
typedef struct {
    seq_t *data;
    size_t size;
    size_t cap;
} SeqArray;

static inline void seq_arr_init(SeqArray *a)
{
    a->data = NULL;
    a->size = 0;
    a->cap  = 0;
}

static inline void seq_arr_push(SeqArray *a, seq_t v)
{
    if (a->size == a->cap) {
        a->cap = a->cap ? a->cap * 2 : 8;
        a->data = (seq_t *)realloc(a->data, a->cap * sizeof(seq_t));
    }
    a->data[a->size++] = v;
}

/* ----------------------------------------------------------- */
/*  Smallest prime factor array (spf).  Rebuilt when needed. */
static uint32_t *spf = NULL;
static uint64_t  spf_limit = 0;

/* Build SPF up to 'limit'.  If limit <= spf_limit nothing is done. */
static void ensure_spf_limit(uint64_t limit)
{
    if (limit <= spf_limit)
        return;

    /* Allocate new SPF array and compute */
    uint32_t *new_spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!new_spf)
        abort();

    for (uint64_t i = 0; i <= limit; ++i)
        new_spf[i] = (uint32_t)i;          /* initially each number is its own spf */

    uint64_t sqrtL = (uint64_t)sqrt((double)limit);
    for (uint64_t i = 2; i <= sqrtL; ++i) {
        if (new_spf[i] == i) {   /* i is prime */
            for (uint64_t j = i * i; j <= limit; j += i) {
                if (new_spf[j] == j)
                    new_spf[j] = (uint32_t)i;
            }
        }
    }

    /* Replace old array */
    free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* ----------------------------------------------------------- */
/*  Factor a number using the SPF array and return distinct primes. */
static inline void factor_distinct_primes(uint32_t n, uint32_t *primes, size_t *cnt)
{
    size_t c = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        primes[c++] = p;
        while (n % p == 0)
            n /= p;
    }
    *cnt = c;
}

/* ----------------------------------------------------------- */
/*  Main sequence function. */
seq_t a(idx_t n)
{
    /* Static state */
    static bool initialized = false;
    static SeqArray results;         /* cached sequence values (1‑based) */
    static uint64_t last_checked = 1;/* last integer that has been tested */
    static uint64_t scan_limit  = 200000; /* current upper bound for SPF */

    if (!initialized) {
        seq_arr_init(&results);
        /* Build initial SPF table */
        ensure_spf_limit(scan_limit);
        initialized = true;
    }

    /* Main loop: generate terms until we have at least 'n'. */
    while (results.size < n) {
        /* If we already finished scanning up to scan_limit, extend the limit. */
        if (last_checked >= scan_limit) {
            /* Double the limit (a moderate growth). */
            scan_limit <<= 1;
            ensure_spf_limit(scan_limit);
        }

        ++last_checked; /* check the next integer */

        /* Quick skip for small numbers: need at least 3 distinct primes. */
        if (last_checked < 5)
            continue;      /* 2,3,4 cannot have ω>2 */

        /* Factor last_checked */
        uint32_t prime_buf[20];
        size_t prime_cnt = 0;
        factor_distinct_primes((uint32_t)last_checked, prime_buf, &prime_cnt);

        if (prime_cnt <= 2)
            continue;      /* need at least 3 distinct primes */

        /* Check if the primes form an arithmetic progression */
        uint32_t diff = prime_buf[1] - prime_buf[0];
        bool is_ap = true;
        for (size_t i = 2; i < prime_cnt; ++i) {
            if (prime_buf[i] - prime_buf[i - 1] != diff) {
                is_ap = false;
                break;
            }
        }
        if (!is_ap)
            continue;

        /* Accept the number */
        seq_arr_push(&results, (seq_t)last_checked);
    }

    /* Return the requested term (offset 1). */
    if (n == 0)
        return 0;  /* undefined, but guard against bad input */
    return results.data[n - 1];
}
