/*
 * A280011  Let d(1) < d(2) < ... < d(q) be the q distinct prime divisors of
 *          an integer m. The sequence lists the numbers m such that d(k) ==
 *          d(k+2) mod d(k+1) for k = 1,2,...,q-2.
 * https://oeis.org/A280011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1002, u32 n <= 1002, u64 n <= 1002,
 *            u128 n <= 1002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A280011: numbers m such that for distinct prime divisors
   d(1) < d(2) < ... < d(q),
   the congruence d(k) == d(k+2) (mod d(k+1)) holds for k = 1..q-2. */

/* The code builds the list of such numbers lazily.
   It uses a simple sieve to generate primes up to 1 000 000,
   tests each integer >= 2 until the requested index is reached,
   and caches the results for future calls.  The routine works for
   all supported seq_t and idx_t types (including arbitrary‑precision
   integer classes), provided “seq_t m; m = n;” is a valid conversion. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/*  Prime generation (simple static sieve)                      */
/* ------------------------------------------------------------ */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt  = 0;

static void init_primes(void)
{
    if (prime_list != NULL) return;   /* already initialized */

    const size_t limit = 1000000; /* upper bound for prime sieve        */
    bool *is_comp = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_comp) exit(EXIT_FAILURE);

    /* estimate upper bound for number of primes up to 1 000 000 */
    size_t est = 80000;
    prime_list = (uint64_t*)malloc(est * sizeof(uint64_t));
    if (!prime_list) exit(EXIT_FAILURE);

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            prime_list[cnt++] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    prime_cnt = cnt;
    prime_list = (uint64_t*)realloc(prime_list, prime_cnt * sizeof(uint64_t));
    if (!prime_list) exit(EXIT_FAILURE);

    free(is_comp);
}

/* ------------------------------------------------------------ */
/*  Test whether m satisfies the triple‑congruence condition   */
/* ------------------------------------------------------------ */
static bool qualifies(uint64_t m)
{
    uint64_t temp = m;
    uint64_t primes_present[64];
    size_t   np = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            primes_present[np++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) primes_present[np++] = temp; /* remaining prime factor */

    if (np < 3) return false;          /* need at least 3 distinct primes */

    for (size_t i = 0; i + 2 < np; ++i) {
        uint64_t p1 = primes_present[i];
        uint64_t p2 = primes_present[i + 1];
        uint64_t p3 = primes_present[i + 2];
        if (p1 % p2 != p3 % p2) return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/*  Sequence cache and generation                              */
/* ------------------------------------------------------------ */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;   /* number of elements stored    */
static size_t seq_cap  = 0;   /* current cache capacity        */
static uint64_t next_candidate = 2;   /* next integer to test          */

static void extend_cache(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_mem = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_mem) exit(EXIT_FAILURE);
    seq_cache = new_mem;
    seq_cap   = new_cap;
}

static void generate_until(idx_t n)  /* generate until a(n) is available */
{
    init_primes();          /* ensure prime list ready */
    while (seq_len < (size_t)n) {
        if (qualifies(next_candidate)) {
            extend_cache(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* ------------------------------------------------------------ */
/*  Public entry point                                         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* sequence is 1‑indexed */
    generate_until(n);
    return seq_cache[n - 1];
}
