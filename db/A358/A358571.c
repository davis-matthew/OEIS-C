/*
 * A358571  Lesser p of a sexy prime pair such that (p-3)/2 is also the lesser
 *          prime of a sexy prime pair.
 * https://oeis.org/A358571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 33, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A358571: the nth lesser prime p of a sexy prime pair (p,p+6)
   such that (p-3)/2 is also the lesser prime of a sexy prime pair. */

/*  The algorithm precomputes primes up to a fixed bound (30 000 050),
    stores a boolean flag table for O(1) primality checks, and
    builds the required sequence lazily.  This is fast enough for
    up to n = 10 000 and works with any seq_t that can hold values up
    to the chosen bound (all fit in 64 bits). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static const uint64_t PRIME_LIMIT = 30000050ULL;   /* must be > any p+6 needed */

static unsigned char *prime_flag = NULL;          /* 1 if prime, 0 otherwise */
static uint64_t *prime_list = NULL;              /* sorted list of primes */
static size_t prime_cnt = 0;                     /* number of primes found */

static seq_t *seq_cache = NULL;                  /* cached result terms */
static size_t seq_len = 0;                       /* number of terms cached     */
static size_t seq_cap = 0;                       /* capacity of seq_cache       */
static size_t prime_idx = 0;                     /* next prime to examine      */

/* Helper: initialise the sieve and prime list once. */
static void init_primes(void)
{
    if (prime_flag) return;                     /* already initialised */

    /* Allocate and initialise the primality flag array. */
    prime_flag = (unsigned char*)malloc(PRIME_LIMIT + 1);
    memset(prime_flag, 1, PRIME_LIMIT + 1);
    prime_flag[0] = prime_flag[1] = 0;

    /* Classic Eratosthenes sieve. */
    uint64_t i, j;
    for (i = 2; i * i <= PRIME_LIMIT; ++i)
        if (prime_flag[i])
            for (j = i * i; j <= PRIME_LIMIT; j += i)
                prime_flag[j] = 0;

    /* Build the prime list. */
    prime_cnt  = 0;
    uint64_t sz = 2000000;                     /* initial capacity  */
    prime_list = (uint64_t*)malloc(sz * sizeof(uint64_t));

    for (i = 2; i <= PRIME_LIMIT; ++i)
        if (prime_flag[i]) {
            if (prime_cnt == sz) {
                sz *= 2;
                prime_list = (uint64_t*)realloc(prime_list, sz * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = i;
        }
}

/* Helper: generate sequence terms until we have at least n terms. */
static void generate_terms_upto(idx_t n)
{
    init_primes();

    if (seq_len >= n) return;

    if (!seq_cache) {
        seq_cap   = 16;
        seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    }

    while (seq_len < n) {
        if (prime_idx >= prime_cnt) break;      /* safety check */

        uint64_t p = prime_list[prime_idx++];
        if (p < 13) continue;                  /* first term is 13 */

        if (!prime_flag[p + 6]) continue;       /* p+6 must be prime */

        uint64_t q = (p - 3) / 2;
        if (q > PRIME_LIMIT || !prime_flag[q]) continue;
        if (q + 6 > PRIME_LIMIT || !prime_flag[q + 6]) continue;

        /* store the valid term */
        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
        }
        seq_cache[seq_len++] = (seq_t)p;
    }
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined, but safe */
    generate_terms_upto(n);
    return seq_cache[n - 1];
}
