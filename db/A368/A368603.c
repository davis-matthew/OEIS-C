/*
 * A368603  Products of odd primes of squarefree index. MM-numbers of set
 *          multipartitions.
 * https://oeis.org/A368603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A368603 – Products of odd primes of square‑free index
    a(n) is the n‑th smallest positive integer whose odd prime
    factors all have square‑free positions in the prime list.
    The function below computes a(n) via the super‑ugly‑numbers
    algorithm.  It memoises previously computed values for
    arbitrary order of calls.  It works for any seq_t/idx_t
    combination provided by the build system. */
    
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Constants */
#define SIEVE_LIMIT   50000   /* enough to cover all needed primes */

/* ----------  helpers  ---------- */

/* test whether k is squarefree (k <= SIEVE_LIMIT) */
static bool is_squarefree(uint64_t k)
{
    if (k <= 1) return true;
    for (uint64_t d = 2; d * d <= k; ++d)
        if (k % (d * d) == 0) return false;
    return true;
}

/* ----------  cache structures  ---------- */
static bool ready = false;

/* primes up to SIEVE_LIMIT (0‑based index) */
static uint64_t primes[SIEVE_LIMIT + 1];
static size_t primes_count;

/* subset of primes: odd primes whose index is squarefree */
static seq_t prime_set[SIEVE_LIMIT + 1];
static size_t prime_set_count;

/* pointers for the super‑ugly algorithm */
static size_t *ptrs;

/* the sequence computed so far */
static seq_t *seq_a = NULL;
static size_t seq_len   = 0;     /* number of elements in seq_a */
static size_t seq_cap   = 0;     /* allocated capacity of seq_a */

/* ----------  initialization  ---------- */
static void init(void)
{
    if (ready) return;

    /* 1. sieve */
    bool *is_comp = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_comp[i] = false;
    primes_count = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[primes_count++] = i;
            if (i * i <= SIEVE_LIMIT) {
                for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);

    /* 2. select odd primes whose 1‑based index is squarefree */
    prime_set_count = 0;
    for (size_t idx = 0; idx < primes_count; ++idx) {
        uint64_t p = primes[idx];
        if ((p & 1ULL) == 0ULL) continue;          /* skip 2 */
        uint64_t pos = idx + 1;                    /* 1‑based position */
        if (is_squarefree(pos))
            prime_set[prime_set_count++] = (seq_t)p;
    }

    /* 3. allocate pointer array */
    ptrs = (size_t *)malloc(prime_set_count * sizeof(size_t));
    for (size_t i = 0; i < prime_set_count; ++i) ptrs[i] = 0;

    /* 4. initialise sequence array with a(1)=1 */
    seq_cap = 16;
    seq_a   = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_a[0] = (seq_t)1;
    seq_len  = 1;

    ready = true;
}

/* extend the sequence up to at least n elements */
static void grow_seq(idx_t n)
{
    if ((size_t)n <= seq_len) return;
    while (seq_len < (size_t)n) {
        seq_t next_val;
        bool first = true;
        /* find minimal candidate */
        for (size_t j = 0; j < prime_set_count; ++j) {
            seq_t cand = prime_set[j] * seq_a[ptrs[j]];
            if (first || cand < next_val) {
                next_val = cand;
                first   = false;
            }
        }
        /* store */
        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq_a = (seq_t *)realloc(seq_a, seq_cap * sizeof(seq_t));
        }
        seq_a[seq_len++] = next_val;
        /* move pointers that produced the minimum */
        for (size_t j = 0; j < prime_set_count; ++j) {
            if (prime_set[j] * seq_a[ptrs[j]] == next_val)
                ++ptrs[j];
        }
    }
}

/* ----------  public interface  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */
    init();
    grow_seq(n);
    return seq_a[n - 1];
}
