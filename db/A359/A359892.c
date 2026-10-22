/*
 * A359892  Members of A026424 (numbers with an odd number of prime factors)
 *          whose prime indices do not have the same mean as median.
 * https://oeis.org/A359892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer >1 that * – has an odd number
 *            of prime factors (counted with multiplicity) (∈A026424), * –
 *            whose multiset of prime indices has mean ≠ median. (intersection
 *            with * A359890 complement)
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The build system supplies the exact types idx_t (>=uint64_t) and seq_t
 * (an unsigned integer type, possibly arbitrary‑precision).  Do not redeclare
 * them.  The implementation below uses only integer operations, no
 * floating‑point.  It precomputes primes up to a fixed limit large enough
 * to generate the first few thousand terms (the required limit is far
 * below the 2 second deadline).  The result is memoised so that a(n) may be
 * called in any order.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Prime infrastructure ---------- */

#define PRIME_LIMIT 2000000   /* large enough for the required range */

static bool primes_done = false;     /* whether the prime data has been prepared */
static uint32_t *primes = NULL;      /* array of primes */
static size_t primes_cnt = 0;        /* number of primes found */

/* prime_index[i] = index of prime i (1‑based), 0 if i is not prime */
static uint32_t *prime_index = NULL;

/* Initialise the prime sieve and prime_index table. */
static void init_primes(void)
{
    if (primes_done)
        return;

    /* Allocate the index lookup table. */
    prime_index = (uint32_t *)calloc(PRIME_LIMIT + 1, sizeof(uint32_t));
    if (!prime_index) {
        exit(EXIT_FAILURE);
    }

    /* Allocate a working array for the sieve. */
    unsigned char *is_comp = (unsigned char *)calloc(PRIME_LIMIT + 1,
                                                    sizeof(unsigned char));
    if (!is_comp) {
        free(prime_index);
        exit(EXIT_FAILURE);
    }

    /* Allocate a provisional array for the primes themselves. */
    size_t primes_cap = PRIME_LIMIT / 10;   /* rough upper bound */
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) {
        free(prime_index);
        free(is_comp);
        exit(EXIT_FAILURE);
    }

    /* Sieve of Eratosthenes. */
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            /* i is prime.  Store it. */
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes,
                                                   primes_cap * sizeof(uint32_t));
                if (!tmp) {
                    free(primes);
                    free(prime_index);
                    free(is_comp);
                    exit(EXIT_FAILURE);
                }
                primes = tmp;
            }
            primes[primes_cnt++] = i;
            prime_index[i] = (uint32_t)(primes_cnt);  /* 1‑based index */

            /* Mark multiples of i as composite. */
            if ((uint64_t)i * (uint64_t)i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    free(is_comp);
    primes_done = true;
}

/* ---------- Sequence generation ---------- */

/* Cache of computed terms. */
static seq_t *terms = NULL;      /* dynamic array */
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Current integer to test next; starts at 2. */
static idx_t next_candidate = 2;

/* Grow the terms array as needed. */
static void grow_terms(void)
{
    if (terms_cap == 0) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) exit(EXIT_FAILURE);
    } else {
        size_t new_cap = terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Helper to test whether a candidate satisfies the sequence condition. */
static bool is_valid(idx_t n)
{
    uint64_t tmp = (uint64_t)n;          /* may overflow if idx_t > 64 bits, but
                                           candidates are small in this context */
    int len = 0;                         /* Ω(n) */
    int sum_indices = 0;
    int indices[32];                     /* small enough for the range considered */

    /* Factorise n by trial division over the precomputed primes. */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > tmp)
            break;
        if (tmp % p == 0) {
            int exp = 0;
            uint32_t idx = prime_index[p];
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
                if (len < 32)          /* indices array small – should fit */
                    indices[len++] = (int)idx;
            }
            sum_indices += idx * exp;
        }
    }

    /* Remaining prime factor (if any). */
    if (tmp > 1) {
        uint32_t idx = prime_index[(uint32_t)tmp];
        if (len < 32)
            indices[len++] = (int)idx;
        sum_indices += idx;
    }

    /* Need an odd number of prime factors. */
    if ((len & 1) == 0)
        return false;

    /* Sort the indices to determine median.  Simple insertion sort
       since len is tiny. */
    for (int i = 1; i < len; ++i) {
        int key = indices[i];
        int j = i - 1;
        while (j >= 0 && indices[j] > key) {
            indices[j + 1] = indices[j];
            --j;
        }
        indices[j + 1] = key;
    }

    /* Compare mean and median.  Avoid floating point by cross‑multiplication. */
    if ((len & 1) == 1) {                 /* odd length */
        int med = indices[len / 2];
        if (sum_indices == med * len)
            return false;                 /* mean == median, reject */
    } else {                              /* even length */
        int L = indices[len / 2 - 1];
        int R = indices[len / 2];
        if (2 * sum_indices == (L + R) * len)
            return false;                 /* mean == median, reject */
    }

    return true;
}

/* Generate terms until we have at least 'target' terms stored. */
static void generate_upto(size_t target)
{
    init_primes();                       /* ensure prime data is ready */

    while (terms_len < target) {
        if (is_valid(next_candidate)) {
            if (terms_len == terms_cap)
                grow_terms();
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* ---------- public interface ---------- */

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined below offset */
        return (seq_t)0;

    /* Ensure we have the first n terms. */
    generate_upto((size_t)n);

    return terms[(size_t)n - 1];
}
